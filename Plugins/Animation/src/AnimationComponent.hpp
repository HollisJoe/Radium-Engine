#ifndef ANIMPLUGIN_ANIMATION_COMPONENT_HPP_
#define ANIMPLUGIN_ANIMATION_COMPONENT_HPP_

#include <AnimationPluginMacros.hpp>

#include <Core/Animation/Pose/Pose.hpp>
#include <Core/Animation/Handle/Skeleton.hpp>
#include <Core/Animation/Animation.hpp>
#include <Core/Animation/Handle/HandleWeight.hpp>

#include <Engine/Component/Component.hpp>
#include <Engine/Assets/HandleData.hpp>
#include <Engine/Assets/AnimationData.hpp>

namespace AnimationPlugin
{

    class SkeletonBoneRenderObject;

    class AnimationComponent : public Ra::Engine::Component
    {
    public:
        AnimationComponent(const std::string& name) :
            Component(name),
            m_animationID( 0 ),
            m_animationTimeStep( true ),
            m_animationTime( 0.0 ),
            m_dt(),
            m_speed( 1.0 ),
            m_slowMo( false ),
            m_selectedBone(-1),
            m_wasReset(false),
            m_resetDone(false)
        {}
        virtual ~AnimationComponent() {}

        virtual void initialize() override{}

        void setSkeleton(const Ra::Core::Animation::Skeleton& skel);

        inline Ra::Core::Animation::Skeleton& getSkeleton() { return m_skel; }
        ANIM_PLUGIN_API Ra::Core::Animation::WeightMatrix getWeights() const;
        ANIM_PLUGIN_API Ra::Core::Animation::Pose getRefPose() const;


        /// Update the skeleton with an animation.
        void update(Scalar dt);
        void reset();
        ANIM_PLUGIN_API void setXray(bool on) const;

        void toggleSkeleton( const bool status );
        void toggleAnimationTimeStep( const bool status );
        void setSpeed( const Scalar value );
        void toggleSlowMotion( const bool status );
        void setAnimation( const uint i );


        void handleSkeletonLoading( const Ra::Asset::HandleData* data, const std::map< uint, uint >& duplicateTable );
        void handleAnimationLoading( const std::vector< Ra::Asset::AnimationData* > data );

        //
        // Editable interface
        //

        virtual bool canEdit(Ra::Core::Index roIdx) const override;

        virtual Ra::Core::Transform getTransform(Ra::Core::Index roIdx) const override;

        virtual void setTransform(Ra::Core::Index roIdx, const Ra::Core::Transform& transform) override;

    private:
        // debug function to display the hierarchy
        void printSkeleton(const Ra::Core::Animation::Skeleton& skeleton);private:

        //
        // Loading data functions
        //

        // Create a skeleton from a file data.
        void createSkeleton( const Ra::Asset::HandleData* data, std::map< uint, uint >& indexTable );

        // Internal recursive method to create bones
        void addBone( const int parent,
                      const uint dataID,
                      const Ra::Core::AlignedStdVector< Ra::Asset::HandleComponentData >& data,
                      const Ra::Core::AlignedStdVector< Ra::Core::Vector2i >& edgeList,
                      std::vector< bool >& processed,
                      std::map< uint, uint >& indexTable );

        // Internal function to create the skinning weights.
        void createWeightMatrix( const Ra::Asset::HandleData* data, const std::map< uint, uint >& indexTable, const std::map< uint, uint >& duplicateTable );

        // Internal function to create the bone display objects.
        void setupSkeletonDisplay();

        // Component communication
        void setupIO( const std::string& id );

        const Ra::Core::Animation::Skeleton*     getSkeletonOutput() const;
        const Ra::Core::Animation::RefPose*      getRefPoseOutput() const;
        const Ra::Core::Animation::WeightMatrix* getWeightsOutput() const;
        const bool*                              getWasReset() const;

    private:
        std::string m_contentName;

        Ra::Core::Animation::Skeleton m_skel; // Skeleton
        Ra::Core::Animation::RefPose m_refPose; // Ref pose in model space.
        std::vector<Ra::Core::Animation::Animation> m_animations;
        Ra::Core::Animation::WeightMatrix m_weights; // Skinning weights ( should go in skinning )

        std::vector<SkeletonBoneRenderObject*> m_boneDrawables ; // Vector of bone display objects
        bool   m_showSkeleton;
        uint   m_animationID;
        bool   m_animationTimeStep;
        Scalar m_animationTime;
        std::vector< Scalar > m_dt;
        Scalar m_speed;
        bool   m_slowMo;

        int m_selectedBone;
        bool m_wasReset;
        bool m_resetDone;
    };

}

#endif //ANIMPLUGIN_ANIMATION_COMPONENT_HPP_
