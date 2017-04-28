#include <Gui/MainWindow.hpp>

#include <QSettings>
#include <QFileDialog>
#include <QToolButton>

#include <Core/Utils/File/OBJFileManager.hpp>
#include <assimp/Importer.hpp>

#include <Engine/RadiumEngine.hpp>
#include <Engine/Component/Component.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/Renderer/Renderer.hpp>
#include <Engine/Renderer/RenderTechnique/RenderTechnique.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderProgram.hpp>
#include <Engine/Renderer/RenderTechnique/ShaderConfigFactory.hpp>

#include <GuiBase/Viewer/CameraInterface.hpp>
#include <GuiBase/TreeModel/EntityTreeModel.hpp>

#include <Gui/MaterialEditor.hpp>
#include <Gui/MainWindow.hpp>

#include <MainApplication.hpp>
#include <PluginBase/RadiumPluginInterface.hpp>


using Ra::Engine::ItemEntry;

namespace Ra
{

    Gui::MainWindow::MainWindow(QWidget* parent)
            : QMainWindow(parent)
    {
        // Note : at this point most of the components (including the Engine) are
        // not initialized. Listen to the "started" signal.

        setupUi(this);

        setWindowIcon(QPixmap(":/Assets/Images/RadiumIcon.png"));
        setWindowTitle(QString("Radium Engine"));

        QStringList headers;
        headers << tr("Entities -> Components");
        m_itemModel = new GuiBase::ItemModel(mainApp->getEngine(), this);
        m_entitiesTreeView->setModel(m_itemModel);
        m_materialEditor = new MaterialEditor();
        m_selectionManager = new GuiBase::SelectionManager(m_itemModel, this);
        m_entitiesTreeView->setSelectionModel(m_selectionManager);

        createConnections();

        mainApp->framesCountForStatsChanged((uint) m_avgFramesCount->value());
        m_trackedPoint = nullptr;
    }

    Gui::MainWindow::~MainWindow()
    {
        // Child QObjects will automatically be deleted
    }

    void Gui::MainWindow::cleanup()
    {
        gizmoShowNone();
    }

    void Gui::MainWindow::createConnections()
    {
        connect(actionOpenMesh, &QAction::triggered, this, &MainWindow::loadFile);
        connect(actionReload_Shaders, &QAction::triggered, m_viewer, &Viewer::reloadShaders);
        connect(actionOpen_Material_Editor, &QAction::triggered, this, &MainWindow::openMaterialEditor);

        // Toolbox setup
        connect(actionToggle_Local_Global, &QAction::toggled, m_viewer->getGizmoManager(), &GizmoManager::setLocal);
        connect(actionGizmoOff, &QAction::triggered, this, &MainWindow::gizmoShowNone);
        connect(actionGizmoTranslate, &QAction::triggered, this, &MainWindow::gizmoShowTranslate);
        connect(actionGizmoRotate, &QAction::triggered, this, &MainWindow::gizmoShowRotate);

        connect(actionRecord_Frames, &QAction::toggled, mainApp, &MainApplication::setRecordFrames);

        // Loading setup.
        connect(this, &MainWindow::fileLoading, mainApp, &BaseApplication::loadFile);

        // Connect picking results (TODO Val : use events to dispatch picking directly)
        connect(m_viewer, &Viewer::rightClickPicking, this, &MainWindow::handlePicking);
        connect(m_viewer, &Viewer::leftClickPicking, m_viewer->getGizmoManager(), &GizmoManager::handlePickingResult);

        connect(m_avgFramesCount, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
                mainApp, &BaseApplication::framesCountForStatsChanged);
        connect(mainApp, &BaseApplication::updateFrameStats, this, &MainWindow::onUpdateFramestats);

        // Inform property editors of new selections
        connect(m_selectionManager, &GuiBase::SelectionManager::selectionChanged, this, &MainWindow::onSelectionChanged);
        //connect(this, &MainWindow::selectedItem, tab_edition, &TransformEditorWidget::setEditable);

        // Make selected item event visible to plugins
        connect(this, &MainWindow::selectedItem, mainApp, &MainApplication::onSelectedItem);
        connect(this, &MainWindow::selectedItem, m_viewer->getGizmoManager(), &GizmoManager::setEditable);
        connect(this, &MainWindow::selectedItem, m_viewer->getGizmoManager(), &GizmoManager::setEditable);


        //Ajout Axel
        // ###TODO###
        // connecter signal du gizmoManager "a bougé" sur un slot de la mainwindow "a bougé"
        // ce slot met à jour la GUI en utilisant le RO suivi pour retrouver le point

        //connect(m_viewer->getGizmoManager(),&GizmoManager::GizmoMouseMove,this,&MainWindow::updateTrackedPointInfo);
        connect(m_viewer,&Viewer::updateTrackedPoint,this,&MainWindow::updateTrackedPointInfo);

        // Enable changing shaders
        connect(m_currentShaderBox, static_cast<void (QComboBox::*)(const QString&)>( &QComboBox::currentIndexChanged ),
                this, &MainWindow::changeRenderObjectShader);

        // RO Stuff
        connect(m_toggleRenderObjectButton, &QPushButton::clicked, this, &MainWindow::toggleVisisbleRO);
        connect(m_editRenderObjectButton, &QPushButton::clicked, this, &MainWindow::editRO);
        connect(m_exportMeshButton, &QPushButton::clicked, this, &MainWindow::exportCurrentMesh);
        connect(m_removeEntityButton, &QPushButton::clicked, this, &MainWindow::deleteCurrentItem);
        connect(m_clearSceneButton, &QPushButton::clicked, this, &MainWindow::resetScene);

        // Renderer stuff

        //connect(m_currentRendererCombo,
        //        static_cast<void (QComboBox::*)(const QString&)>( &QComboBox::currentIndexChanged ),
        //        this, &MainWindow::changeRenderer);

        connect(m_viewer, &Viewer::rendererReady, this, &MainWindow::onRendererReady);

        connect(m_displayedTextureCombo,
                static_cast<void (QComboBox::*)(const QString&)>( &QComboBox::currentIndexChanged ),
                m_viewer, &Viewer::displayTexture);

        connect(m_enablePostProcess, &QCheckBox::stateChanged, m_viewer, &Viewer::enablePostProcess);
        connect(m_enableDebugDraw,   &QCheckBox::stateChanged, m_viewer, &Viewer::enableDebugDraw);
        connect(m_realFrameRate,     &QCheckBox::stateChanged, mainApp,  &BaseApplication::setRealFrameRate);

        // Connect engine signals to the appropriate callbacks
        std::function<void(const Engine::ItemEntry&)> add = std::bind(&MainWindow::onItemAdded, this, std::placeholders::_1);
        std::function<void(const Engine::ItemEntry&)> del = std::bind(&MainWindow::onItemRemoved, this, std::placeholders::_1);
        mainApp->m_engine->getSignalManager()->m_entityCreatedCallbacks.push_back(add);
        mainApp->m_engine->getSignalManager()->m_entityDestroyedCallbacks.push_back(del);

        mainApp->m_engine->getSignalManager()->m_componentAddedCallbacks.push_back(add);
        mainApp->m_engine->getSignalManager()->m_componentRemovedCallbacks.push_back(del);

        mainApp->m_engine->getSignalManager()->m_roAddedCallbacks.push_back(add);
        mainApp->m_engine->getSignalManager()->m_roRemovedCallbacks.push_back(del);

    }

    void Gui::MainWindow::loadFile()
    {
        // Filter the files
        aiString extList;
        Assimp::Importer importer;
        importer.GetExtensionList(extList);
        std::string extListStd(extList.C_Str());
        std::replace(extListStd.begin(), extListStd.end(), ';', ' ');
        QString filter =""/* QString::fromStdString(extListStd)*/;

        QSettings settings;
        QString path = settings.value("files/load", QDir::homePath()).toString();
        path = QFileDialog::getOpenFileName(this, "Open File", path, filter);
        if (path.size() > 0)
        {
            settings.setValue("files/load", path);
            emit fileLoading(path);
        }
    }

    void Gui::MainWindow::onUpdateFramestats(const std::vector<FrameTimerData>& stats)
    {
        QString framesA2B = QString("Frames #%1 to #%2 stats :")
                .arg(stats.front().numFrame).arg(stats.back().numFrame);
        m_frameA2BLabel->setText(framesA2B);


        auto romgr = mainApp->m_engine->getRenderObjectManager();

        uint polycount = romgr->getNumFaces();
        uint vertexcount = romgr->getNumVertices();


        QString polyCountText = QString("Rendering %1 faces and %2 vertices").arg(polycount).arg(vertexcount);
        m_labelCount->setText(polyCountText);

        long sumEvents = 0;
        long sumRender = 0;
        long sumTasks = 0;
        long sumFrame = 0;
        long sumInterFrame = 0;

        for (uint i = 0; i < stats.size(); ++i)
        {
            sumEvents += Core::Timer::getIntervalMicro(stats[i].eventsStart, stats[i].eventsEnd);
            sumRender += Core::Timer::getIntervalMicro(stats[i].renderData.renderStart, stats[i].renderData.renderEnd);
            sumTasks += Core::Timer::getIntervalMicro(stats[i].tasksStart, stats[i].tasksEnd);
            sumFrame += Core::Timer::getIntervalMicro(stats[i].frameStart, stats[i].frameEnd);

            if (i > 0)
            {
                sumInterFrame += Core::Timer::getIntervalMicro(stats[i - 1].frameEnd, stats[i].frameEnd);
            }
        }

        const uint N(stats.size());
        const Scalar T(N * 1000000.f);

        m_eventsTime->setValue(sumEvents / N);
        m_eventsUpdates->setValue(T / Scalar(sumEvents));
        m_renderTime->setValue(sumRender / N);
        m_renderUpdates->setValue(T / Scalar(sumRender));
        m_tasksTime->setValue(sumTasks / N);
        m_tasksUpdates->setValue(T / Scalar(sumTasks));
        m_frameTime->setValue(sumFrame / N);
        m_frameUpdates->setValue(T / Scalar(sumFrame));
        m_avgFramerate->setValue((N - 1) * Scalar(1000000.0 / sumInterFrame));
    }

    Gui::Viewer* Gui::MainWindow::getViewer()
    {
        return m_viewer;
    }

    GuiBase::SelectionManager* Gui::MainWindow::getSelectionManager()
    {
        return m_selectionManager;
    }

    //Ajout Axel : type bool en parametre
    void Gui::MainWindow::handlePicking(int pickingResult, bool ctrl)
    {
        Ra::Core::Index roIndex(pickingResult);
        Ra::Engine::RadiumEngine* engine = Ra::Engine::RadiumEngine::getInstance();
        if (roIndex.isValid())
        {
            auto ro = engine->getRenderObjectManager()->getRenderObject(roIndex);
            if ( ro->getType() != Ra::Engine::RenderObjectType::UI)
            {
                Ra::Engine::Component* comp = ro->getComponent();
                Ra::Engine::Entity* ent = comp->getEntity();

                // For now we don't enable group selection.
                m_selectionManager->setCurrentEntry(ItemEntry(ent, comp, roIndex),
                                                    QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);


                //Ajout Axel

                // ###TODO###
                // si il y a une selection
                // récupérer l'indice du premier point du mesh
                // m-a-j de l'interface


                if (ctrl)
                {
                    m_trackedPoint = &(ro ->getMesh()->getGeometry().m_vertices[0]);
                    updateTrackedPointInfo();
                }
                // enregistrer le RO à suivre

            }
        }
        else
        {
            m_selectionManager->clear();
        }
    }

    void Gui::MainWindow::onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
    {
        if (m_selectionManager->hasSelection())
        {
            const ItemEntry& ent = m_selectionManager->currentItem();
            emit selectedItem(ent);
            m_selectedItemName->setText(QString::fromStdString(getEntryName(mainApp->getEngine(), ent)));
            m_editRenderObjectButton->setEnabled(false);

            if (ent.isRoNode())
            {
                m_editRenderObjectButton->setEnabled(true);

                m_materialEditor->changeRenderObject(ent.m_roIndex);
                const std::string& shaderName = mainApp->m_engine->getRenderObjectManager()
                                                       ->getRenderObject(ent.m_roIndex)->getRenderTechnique()
                                                       ->shader->getBasicConfiguration().m_name;

                if (m_currentShaderBox->findText(shaderName.c_str()) == -1)
                {
                    m_currentShaderBox->addItem(QString(shaderName.c_str()));
                    m_currentShaderBox->setCurrentText(shaderName.c_str());
                }
                else
                {
                    m_currentShaderBox->setCurrentText(shaderName.c_str());
                }
            }
        }
        else
        {
            emit selectedItem(ItemEntry());
            m_selectedItemName->setText("");
            m_editRenderObjectButton->setEnabled(false);
            m_materialEditor->hide();
        }
    }

    void Gui::MainWindow::closeEvent(QCloseEvent* event)
    {
        emit closed();
        event->accept();
    }

    void Gui::MainWindow::gizmoShowNone()
    {
        m_viewer->getGizmoManager()->changeGizmoType(GizmoManager::NONE);
    }

    void Gui::MainWindow::gizmoShowTranslate()
    {
        m_viewer->getGizmoManager()->changeGizmoType(GizmoManager::TRANSLATION);
    }

    void Gui::MainWindow::gizmoShowRotate()
    {
        m_viewer->getGizmoManager()->changeGizmoType(GizmoManager::ROTATION);
    }

    void Gui::MainWindow::changeRenderer(const QString& rendererName)
    {
        // m_viewer->changeRenderer(m_currentRendererCombo->currentIndex());
    }


    void Gui::MainWindow::changeRenderObjectShader(const QString& shaderName)
    {
        std::string name = shaderName.toStdString();
        if (name == "")
        {
            return;
        }

        const ItemEntry& item = m_selectionManager->currentItem();
        const Engine::ShaderConfiguration config = Ra::Engine::ShaderConfigurationFactory::getConfiguration(name);

        auto vector_of_ros = getItemROs( mainApp->m_engine.get(), item );
        for (const auto& ro_index : vector_of_ros) {
            const auto& ro = mainApp->m_engine->getRenderObjectManager()->getRenderObject(ro_index);
            if (ro->getRenderTechnique()->shader->getBasicConfiguration().m_name != name)
            {
                ro->getRenderTechnique()->changeShader(config);
            }
        }
    }

    void Gui::MainWindow::toggleVisisbleRO()
    {
        const ItemEntry& item = m_selectionManager->currentItem();
        // If at least one RO is visible, turn them off.
        bool hasVisible = false;
        for (auto roIdx : getItemROs(mainApp->m_engine.get(), item))
        {
            if (mainApp->m_engine->getRenderObjectManager()->getRenderObject(roIdx)->isVisible())
            {
                hasVisible = true;
                break;
            }
        }
        for (auto roIdx : getItemROs(mainApp->m_engine.get(), item))
        {
            mainApp->m_engine->getRenderObjectManager()->getRenderObject(roIdx)->setVisible(!hasVisible);
        }
    }

    void Gui::MainWindow::editRO()
    {
        ItemEntry item = m_selectionManager->currentItem();
        if (item.isRoNode())
        {
            m_materialEditor->changeRenderObject(item.m_roIndex);
            m_materialEditor->show();
        }
    }

    void Gui::MainWindow::openMaterialEditor()
    {
        m_materialEditor->show();
    }

    void Gui::MainWindow::updateUi(Plugins::RadiumPluginInterface* plugin)
    {
        QString tabName;

        // Add menu
        if (plugin->doAddMenu())
        {
            QMainWindow::menuBar()->addMenu(plugin->getMenu());
        }

        // Add widget
        if (plugin->doAddWidget(tabName))
        {
            toolBox->addTab(plugin->getWidget(), tabName);
        }

        // Add actions
        int nbActions;
        if (plugin->doAddAction( nbActions ))
        {
            for (int i=0; i<nbActions; ++i)
            {
                toolBar->insertAction( 0, plugin->getAction(i) );
            }
            toolBar->addSeparator();
        }
    }

    void Gui::MainWindow::onRendererReady()
    {
        m_viewer->getCameraInterface()->resetCamera();

        QSignalBlocker blockTextures(m_displayedTextureCombo);

        auto texs = m_viewer->getRenderer()->getAvailableTextures();
        for (const auto& tex : texs)
        {
            m_displayedTextureCombo->addItem(tex.c_str());
        }
    }

    void Gui::MainWindow::onFrameComplete()
    {
        tab_edition->updateValues();
        m_viewer->getGizmoManager()->updateValues();
    }

    void Gui::MainWindow::onItemAdded(const Engine::ItemEntry& ent)
    {
        m_itemModel->addItem(ent);
    }

    void Gui::MainWindow::onItemRemoved(const Engine::ItemEntry& ent)
    {
        m_itemModel->removeItem(ent) ;
    }

    void Gui::MainWindow::exportCurrentMesh()
    {
        std::string filename;
        Ra::Core::StringUtils::stringPrintf(filename, "radiummesh_%06u", mainApp->getFrameCount());
        ItemEntry e = m_selectionManager->currentItem();

        // For now we only export a mesh if the selected entry is a render object.
        // There could be a virtual method to get a mesh representation for any object.
        if (e.isRoNode())
        {
            Ra::Core::OBJFileManager obj;
            auto ro = Engine::RadiumEngine::getInstance()->getRenderObjectManager()->getRenderObject(e.m_roIndex);
            Ra::Core::TriangleMesh mesh = ro->getMesh()->getGeometry();
            bool result = obj.save( filename, mesh );
            if (result)
            {
                LOG(logINFO)<<"Mesh from "<<ro->getName()<<" successfully exported to "<<filename;
            }
            else
            {
                LOG(logERROR)<<"Mesh from "<<ro->getName()<<"failed to export";
            }
        }
        else
        {
            LOG(logWARNING)<< "Current entry was not a render object. No mesh was exported.";
        }
    }

    void Gui::MainWindow::deleteCurrentItem()
    {
        ItemEntry e = m_selectionManager->currentItem();

        // This call is very important to avoid a potential race condition
        // which happens if an object is selected while a gizmo is present.
        // If we do not do this, the removal of the object will call ItemModel::removeItem() which
        // will cause it to be unselected by the selection model. This in turn will cause
        // the gizmos ROs to disappear, but the RO mutex is already acquired by the call for
        // the object we want to delete, which causes a deadlock.
        // Clearing the selection before deleting the object will avoid this problem.
        m_selectionManager->clear();
        if (e.isRoNode())
        {
            e.m_component->removeRenderObject(e.m_roIndex);
        }
        else if (e.isComponentNode())
        {
            e.m_entity->removeComponent(e.m_component->getName());
        }
        else if (e.isEntityNode())
        {
            Engine::RadiumEngine::getInstance()->getEntityManager()->removeEntity(e.m_entity->idx);
        }
    }

    void Gui::MainWindow::resetScene()
    {
        // To see why this call is important, please see deleteCurrentItem().
        m_selectionManager->clearSelection();
        Engine::RadiumEngine::getInstance()->getEntityManager()->deleteEntities();
        m_viewer->resetCamera();
    }

    void Gui::MainWindow::on_actionForward_triggered()
    {
        changeRenderer("Forward");
        actionForward->setChecked( true );
        actionDeferred->setChecked( false );
        actionExperimental->setChecked( false );
    }

    void Gui::MainWindow::on_actionDeferred_triggered()
    {
        changeRenderer("Deferred");
        actionForward->setChecked( false );
        actionDeferred->setChecked( true );
        actionExperimental->setChecked( false );
    }

    void Ra::Gui::MainWindow::on_actionExperimental_triggered()
    {
        changeRenderer("Experimental");
        actionForward->setChecked( false );
        actionDeferred->setChecked( false );
        actionDeferred->setChecked( true );
    }

    //Ajout Axel

    void Gui::MainWindow::updateTrackedPointInfo()
    {
        if (m_trackedPoint)
        {
            m_valueX -> setText(QString::fromStdString(std::to_string((*m_trackedPoint)[0])));
            m_valueY -> setText(QString::fromStdString(std::to_string((*m_trackedPoint)[1])));
            m_valueZ -> setText(QString::fromStdString(std::to_string((*m_trackedPoint)[2])));
        }
    }

} // namespace Ra
