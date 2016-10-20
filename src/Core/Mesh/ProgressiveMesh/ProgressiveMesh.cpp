#include "ProgressiveMesh.hpp"

#include <Core/RaCore.hpp>

#include <Core/Log/Log.hpp>

#include <Core/Mesh/Wrapper/Convert.hpp>
#include <Core/Mesh/DCEL/Vertex.hpp>
#include <Core/Mesh/DCEL/HalfEdge.hpp>
#include <Core/Mesh/DCEL/FullEdge.hpp>
#include <Core/Mesh/DCEL/Dcel.hpp>
#include <Core/Mesh/DCEL/Operations/EdgeCollapse.hpp>
#include <Core/Mesh/DCEL/Operations/VertexSplit.hpp>
#include <Core/Mesh/ProgressiveMesh/PriorityQueue.hpp>

#include <Core/Geometry/Triangle/TriangleOperation.hpp>

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define triang(x) (((x) * ((x) + 1)) / 2)
#define ind(row, col, n) ((row) * ((n) - 2) - triang((row) - 1) + (col) - 1)

namespace Ra
{
    namespace Core
    {

        ProgressiveMesh::ProgressiveMesh(TriangleMesh* mesh)
        {
            m_dcel = new Dcel();
            m_quadrics = new Quadric[mesh->m_triangles.size()]();
            m_nb_faces = mesh->m_triangles.size();
            m_nb_vertices = mesh->m_vertices.size();
            convert(*mesh, *m_dcel);
        }

        //------------------------------------------------

        void ProgressiveMesh::edgeFaceAdjacency(Index halfEdgeIndex, std::vector<Index>& adjOut)
        {
            // Beware : particular cases for going through all the adjacent faces
            // = : border
            /*
                                  A                                     A
                        \\      /   \       //       |        \\      /   \       //
                         \\    /     \     //        |         \\    /     \     //
                          \\  /       \   //         |          \\  /       \   //
                           \\/         \ //          |           \\/         \ //
                            V1— — — — — V2           |            V1==========V2
                           //\         / \\          |
                          //  \       /   \\         |
                         //    \     /     \\        |
                        //      \   /       \\       |
                                  B
              */

            HalfEdge_ptr h1 = m_dcel->m_halfedge[halfEdgeIndex];
            HalfEdge_ptr h2 = h1->Twin();

            Face_ptr f1, f2;

            if (h1 != NULL)
            {
                f1 = h1->F();
                adjOut.push_back(f1->idx);
                h1 = h1->Next()->Twin();
            }
            if (h2 != NULL)
            {
                f2 = h2->F();
                adjOut.push_back(f2->idx);
                h2 = h2->Prev()->Twin();
            }

            // First path around v2
            while(h1 != NULL || h2 != NULL)
            {
                if (h1 != NULL)
                {
                    f1 = h1->F();
                    if (f1 == f2) break;
                    adjOut.push_back(f1->idx);
                    h1 = h1->Next()->Twin();
                }
                if (h2 != NULL)
                {
                    f2 = h2->F();
                    if (f2 == f1) break;
                    adjOut.push_back(f2->idx);
                    h2 = h2->Prev()->Twin();
                }
            }

            // Second path around v1
            h1 = m_dcel->m_halfedge[halfEdgeIndex];
            if (h1->Twin() != NULL) h2 = h1->Twin()->Next()->Twin();
            else h2 = nullptr;
            h1 = h1->Prev()->Twin();
            while(h1 != NULL || h2 != NULL)
            {
                if (h1 != NULL)
                {
                    f1 = h1->F();
                    if (f1 == f2) break;
                    adjOut.push_back(f1->idx);
                    h1 = h1->Prev()->Twin();
                }
                if (h2 != NULL)
                {
                    f2 = h2->F();
                    if (f2 == f1) break;
                    adjOut.push_back(f2->idx);
                    h2 = h2->Next()->Twin();
                }
            }
        }

        void ProgressiveMesh::vertexFaceAdjacency(Index vertexIndex, std::vector<Index>& adjOut)
        {
            // Beware : particular cases for going through all the adjacent faces
            // see edgeFaceAdjacency function

            Vertex_ptr v = m_dcel->m_vertex[vertexIndex];
            HalfEdge_ptr h1 = v->HE();
            HalfEdge_ptr h2 = h1 != NULL ? h1->Twin() : NULL;

            Face_ptr f1, f2;

            if (h1 != NULL)
            {
                f1 = h1->F();
                adjOut.push_back(f1->idx);
                h1 = h1->Prev()->Twin();
            }
            if (h2 != NULL)
            {
                f2 = h2->F();
                adjOut.push_back(f2->idx);
                h2 = h2->Next()->Twin();
            }

            while(h1 != NULL || h2 != NULL)
            {
                if (h1 != NULL)
                {
                    f1 = h1->F();
                    if (f1 == f2) break;
                    adjOut.push_back(f1->idx);
                    h1 = h1->Prev()->Twin();
                }
                if (h2 != NULL)
                {
                    f2 = h2->F();
                    if (f2 == f1) break;
                    adjOut.push_back(f2->idx);
                    h2 = h2->Next()->Twin();
                }
            }
        }

        void ProgressiveMesh::vertexVertexAdjacency(Index vertexIndex, std::vector<Index>& adjOut)
        {
            // Beware : particular cases for going through all the adjacent vertices
            // see edgeFaceAdjacency function

            Vertex_ptr v = m_dcel->m_vertex[vertexIndex];
            HalfEdge_ptr h1 = v->HE();
            HalfEdge_ptr h2 = h1 != NULL ? h1->Prev()->Twin() : NULL;

            Vertex_ptr v1, v2;

            if (h1 != NULL)
            {
                CORE_ASSERT(h1->V()->idx == vertexIndex, "pb vertex vertex adjacency");
                v1 = h1->Next()->V();
                adjOut.push_back(v1->idx);
                h1 = h1->Twin()->Next();
            }
            if (h2 != NULL)
            {
                CORE_ASSERT(h2->V()->idx == vertexIndex, "pb vertex vertex adjacency");
                v2 = h2->Next()->V();
                adjOut.push_back(v2->idx);
                h2 = h2->Prev()->Twin();
            }

            while(h1 != NULL || h2 != NULL)
            {
                if (h1 != NULL)
                {
                    CORE_ASSERT(h1->V()->idx == vertexIndex, "pb vertex vertex adjacency");
                    v1 = h1->Next()->V();
                    if (v1 == v2) break;
                    adjOut.push_back(v1->idx);
                    h1 = h1->Twin()->Next();
                }
                if (h2 != NULL)
                {
                    CORE_ASSERT(h2->V()->idx == vertexIndex, "pb vertex vertex adjacency");
                    v2 = h2->Next()->V();
                    if (v2 == v1) break;
                    adjOut.push_back(v2->idx);
                    h2 = h2->Prev()->Twin();
                }
            }

        }

        //------------------------------------------------

        void ProgressiveMesh::computeFacesQuadrics()
        {
            const uint numTriangles = m_dcel->m_face.size();

            Vector3 n;
#pragma omp parallel for private(n)
            for (uint t = 0; t < numTriangles; ++t)
            {
                Face_ptr f = m_dcel->m_face[t];
                Vertex_ptr v0 = f->HE()->V();
                Vertex_ptr v1 = f->HE()->Next()->V();
                Vertex_ptr v2 = f->HE()->Next()->Next()->V();

                n = Geometry::triangleNormal(v0->P(), v1->P(), v2->P());

                m_quadrics[t] = Quadric(n, -n.dot(v0->P()));
            }
        }

        void ProgressiveMesh::updateFacesQuadrics(Index vsIndex)
        {
            // We go all over the faces which contain vsIndex
            std::vector<Index> adjFaces;
            vertexFaceAdjacency(vsIndex, adjFaces);

            Vector3 n;
            for (uint t = 0; t < adjFaces.size(); ++t)
            {
                Face_ptr f = m_dcel->m_face[ adjFaces[t] ];
                Vertex_ptr v0 = f->HE()->V();
                Vertex_ptr v1 = f->HE()->Next()->V();
                Vertex_ptr v2 = f->HE()->Next()->Next()->V();

                n = Geometry::triangleNormal(v0->P(), v1->P(), v2->P());

                m_quadrics[adjFaces[t]] = Quadric(n,-n.dot(v0->P()));
            }

        }


        Scalar ProgressiveMesh::getWedgeAngle(Index faceIndex, Index vsIndex, Index vtIndex)
        {
            Scalar wedgeAngle;
            Face_ptr face = m_dcel->m_face[faceIndex];
            Vertex_ptr vs = m_dcel->m_vertex[vsIndex];
            Vertex_ptr vt = m_dcel->m_vertex[vtIndex];

            HalfEdge_ptr he = face->HE();
            for (int i = 0; i < 3; i++)
            {
                if (he->V() == vs || he->V() == vt)
                {
                    Vector3 v0 = he->Next()->V()->P() - he->V()->P();
                    Vector3 v1 = he->Prev()->V()->P() - he->V()->P();
                    v0.normalize();
                    v1.normalize();
                    wedgeAngle = std::acos(v0.dot(v1));
                    break;
                }
                he = he->Next();
            }
            return wedgeAngle;
        }

        Ra::Core::Quadric ProgressiveMesh::computeEdgeQuadric(Index halfEdgeIndex)
        {
            std::vector<Index> adjFaces;
            edgeFaceAdjacency(halfEdgeIndex, adjFaces);

            // We go all over the faces which contain vs and vt
            // We add the quadrics of all the faces
            Quadric q;

            for (unsigned int i = 0; i < adjFaces.size(); i++)
            {
                Face_ptr f = m_dcel->m_face[adjFaces[i]];
//                Scalar area = Ra::Core::Geometry::triangleArea
//                                ( f->HE()->V()->P(),
//                                  f->HE()->Next()->V()->P(),
//                                  f->HE()->Prev()->V()->P());
                Scalar wedgeAngle = getWedgeAngle(adjFaces[i],
                                                m_dcel->m_halfedge[halfEdgeIndex]->V()->idx,
                                                m_dcel->m_halfedge[halfEdgeIndex]->Next()->V()->idx);

                q += m_quadrics[ adjFaces[i] ] * wedgeAngle;
            }

            return q; // * (1.0/double(adjFaces.size()));
        }

        //-----------------------------------------------------

        Scalar ProgressiveMesh::computeEdgeError(Index halfEdgeIndex, Vector3 &p_result)
        {
            Quadric q = computeEdgeQuadric(halfEdgeIndex);
            Scalar error;

            // on cherche v_result
            // A v_result = -b		avec A = nn^T
            //							 b = dn
            Matrix3 A_inverse = q.getA().inverse();

            Scalar det = q.getA().determinant();
            if (det > 0.0001)
            {
                p_result = -A_inverse * q.getB();
                error = computeGeometricError(p_result, q);
            }
            else //matrix non inversible
            {
                Vector3 p1 = m_dcel->m_halfedge[halfEdgeIndex]->V()->P();
                Vector3 p2 = m_dcel->m_halfedge[halfEdgeIndex]->Next()->V()->P();
                Vector3 p12 = (p1 + p2) / 2.0;

                Scalar p1_error = computeGeometricError(p1, q);
                Scalar p2_error = computeGeometricError(p2, q);
                Scalar p12_error = computeGeometricError(p12, q);

                error = p1_error;
                Vector3 p = p1;
                if (p2_error < error && p12_error > p2_error)
                {
                    p = p2;
                    p_result = p;
                    error = p2_error;
                }
                else if (p12_error < error && p2_error > p12_error)
                {
                    p = p12;
                    p_result = p;
                    error = p12_error;
                }
                else
                {
                    p_result = p;
                }
            }
            return error;
        }


        Scalar ProgressiveMesh::computeGeometricError(const Vector3& p, Quadric q)
        {
            // Computing geometric error
            // v^T A v + 2 * b^T v + c
            Eigen::Matrix<Scalar, 1, 3> row_p = p.transpose();
            Eigen::Matrix<Scalar, 1, 3> row_b = q.getB().transpose();
            Scalar error_a = row_p * q.getA() * p;
            Scalar error_b = 2.0 * row_b * p;
            Scalar error_c = q.getC();
            return (error_a + error_b + error_c);
        }

        //--------------------------------------------------

        PriorityQueue ProgressiveMesh::constructPriorityQueue()
        {
            PriorityQueue pQueue = PriorityQueue();

            const uint numTriangles = m_dcel->m_face.size();
            const uint numVertices = m_dcel->m_vertex.size();

            pQueue.reserve(numTriangles*3 / 2);

            // matrice triangulaire inferieure sans diagonale dans un tableau 1D
            // pour eviter de mettre deux fois la meme arete dans la priority queue
            // index = row * (N - 2) - t(row - 1) + col - 1     où t(a) = a*(a+1) / 2
            //    e0  e1  e2
            //  e0    x   x
            //  e1        x
            //  e2
            // Il y a surement moyen de faire quelquechose de plus intelligent...
            int edgeProcessedSize = ind(numVertices - 1, numVertices - 1, numVertices) + 1;
            std::vector<bool> edgeProcessed(edgeProcessedSize, false);

            // parcours des aretes
            uint edgeProcessedInd;
            double edgeError;
            Vector3 p = Vector3::Zero();
            int j;

#pragma omp parallel for private(j, edgeProcessedInd, edgeError, p)
            for (unsigned int i = 0; i < numTriangles; i++)
            {
                const Face_ptr& f = m_dcel->m_face.at( i );
                HalfEdge_ptr h = f->HE();
                for (j = 0; j < 3; j++)
                {
                    const Vertex_ptr& vs = h->V();
                    const Vertex_ptr& vt = h->Next()->V();
                    edgeProcessedInd = ind(min((vs->idx).getValue(), (vt->idx).getValue()),
                                           max((vs->idx).getValue(), (vt->idx).getValue()), numVertices);
                    if (!edgeProcessed[edgeProcessedInd])
                    {
                        edgeError = computeEdgeError(f->HE()->idx, p);

#pragma omp critical
                        {
                            edgeProcessed[edgeProcessedInd] = true;
                            pQueue.insert(PriorityQueue::PriorityQueueData(vs->idx, vt->idx, h->idx, i, edgeError, p));
                        }
                    }
                    h = h->Next();
                }
            }

            return pQueue;
        }

        void ProgressiveMesh::updatePriorityQueue(PriorityQueue &pQueue, Index vsIndex, Index vtIndex)
        {
            // we delete of the priority queue all the edge containing vs_id or vt_id
            pQueue.removeEdges(vsIndex);
            pQueue.removeEdges(vtIndex);

            double edgeError;
            Vector3 p = Vector3::Zero();
            Index vIndex;

            // Adding an edge
            Vertex_ptr vs = m_dcel->m_vertex[vsIndex];
            HalfEdge_ptr h_start = vs->HE();
            edgeError = computeEdgeError(h_start->idx, p);
            vIndex = h_start->Next()->V()->idx;
            pQueue.insert(PriorityQueue::PriorityQueueData(vsIndex, vIndex, h_start->idx, h_start->F()->idx, edgeError, p));

            // Adding the other edges around vs
            HalfEdge_ptr h = h_start->Twin()->Next();
            // TODO if there is a hole
            while(h != h_start)
            {
                edgeError = computeEdgeError(h->idx, p);
                vIndex = h->Next()->V()->idx;

                pQueue.insert(PriorityQueue::PriorityQueueData(vsIndex, vIndex, h->idx, h->F()->idx, edgeError, p));

                h = h->Twin()->Next();
                CORE_ASSERT(h->V()->idx == vsIndex, "Invalid reference vertex");
            }
            //pQueue.display();
        }

        void ProgressiveMesh::test(Index &vs, Index &vt)
        {
            vs = m_dcel->m_halfedge[2]->V()->idx;
            vt = m_dcel->m_halfedge[2]->Next()->V()->idx;
            Vector3 p = (m_dcel->m_halfedge[2]->V()->P() + m_dcel->m_halfedge[2]->Next()->V()->P()) / 2.;
            DcelOperations::edgeCollapse(*m_dcel, 2, p);
        }

        //--------------------------------------------------

        bool ProgressiveMesh::isEcolPossible(Index halfEdgeIndex, Vector3 pResult)
        {
            HalfEdge_ptr he = m_dcel->m_halfedge[halfEdgeIndex];

            // Look at configuration T inside a triangle
            bool hasTIntersection = false;
            std::vector<Index> adjVerticesV1, adjVerticesV2;
            vertexVertexAdjacency(he->V()->idx, adjVerticesV1);
            vertexVertexAdjacency(he->Next()->V()->idx, adjVerticesV2);

            uint countIntersection = 0;
            for (uint i = 0; i < adjVerticesV1.size(); i++)
            {
                for (uint j = 0; j < adjVerticesV2.size(); j++)
                {
                    if (adjVerticesV1[i] == adjVerticesV2[j])
                        countIntersection++;
                }
            }
            if (countIntersection > 2)
                hasTIntersection = true;

            // Look if normals of faces change after collapse
            bool isFlipped = false;
            std::vector<Index> adjFaces;
            edgeFaceAdjacency(he->idx, adjFaces);
            Index vsId = he->V()->idx;
            Index vtId = he->Next()->V()->idx;
            for (uint i = 0; i < adjFaces.size(); i++)
            {
                HalfEdge_ptr heCurr = m_dcel->m_face[adjFaces[i]]->HE();
                Vertex_ptr v1 = nullptr;
                Vertex_ptr v2 = nullptr;
                Vertex_ptr v = nullptr;
                for (uint j = 0; j < 3; j++)
                {
                    if (heCurr->V()->idx != vsId && heCurr->V()->idx != vtId)
                    {
                        if (v1 == nullptr)
                            v1 = heCurr->V();
                        else if (v2 == nullptr)
                            v2 = heCurr->V();
                    }
                    else
                    {
                        v = heCurr->V();
                    }
                    heCurr = heCurr->Next();
                }
                if (v1 != nullptr && v2 != nullptr)
                {
                    Vector3 d1 = v1->P() - pResult;
                    Vector3 d2 = v2->P() - pResult;
                    d1.normalize();
                    d2.normalize();

                    //TEST
                    //A-t'on besoin de ça ?
                    /*
                    Scalar a = fabs(d1.dot(d2));
                    Vector3 d1_before = v1->P() - v->P();
                    Vector3 d2_before = v2->P() - v->P();
                    d1_before.normalize();
                    d2_before.normalize();
                    Scalar a_before = fabs(d1_before.dot(d2_before));
                    if (a > 0.999 && a_before < 0.999)
                        isFlipped = true;
                    */

                    Vector3 fp_n = d1.cross(d2);
                    fp_n.normalize();
                    Vector3 f_n = Geometry::triangleNormal(v->P(), v1->P(), v2->P());
                    if (fp_n.dot(f_n) < 0.1)
                        isFlipped = true;

                }
            }

            return ((!hasTIntersection) && (!isFlipped));
        }

        //--------------------------------------------------

        std::vector<ProgressiveMeshData> ProgressiveMesh::constructM0(int targetNbFaces, int &nbNoFrVSplit)
        {
            uint nbPMData = 0;

            //m_pmdata = new ProgressiveMeshData[nbFullEdges];
            std::vector<ProgressiveMeshData> pmdata;

            LOG(logINFO) << "Computing Faces Quadrics";
            computeFacesQuadrics();

            LOG(logINFO) << "Computing Priority Queue";
            PriorityQueue pQueue = constructPriorityQueue();
            PriorityQueue::PriorityQueueData d;

            LOG(logINFO) << "Collapsing...";
            // while we do not have 'targetNbFaces' faces
            std::vector<Index> adjFaces;
            ProgressiveMeshData pmData;
            while (m_nb_faces > targetNbFaces)
            {
                if (pQueue.empty()) break;
                d = pQueue.top();

                HalfEdge_ptr he = m_dcel->m_halfedge[d.m_edge_id];

                // TODO !
                if (!isEcolPossible(he->idx, d.m_p_result))
                {
                    LOG(logINFO) << "This edge is not collapsable for now";
                    continue;
                }

                if (he->Twin() == nullptr)
                {
                    m_nb_faces -= 1;
                    nbNoFrVSplit++;
                }
                else
                {
                    m_nb_faces -= 2;
                }
                m_nb_vertices -= 1;

                edgeFaceAdjacency(d.m_edge_id, adjFaces);

                pmData = DcelOperations::edgeCollapse(*m_dcel, d.m_edge_id, d.m_p_result);
                updateFacesQuadrics(d.m_vs_id);
                updatePriorityQueue(pQueue, d.m_vs_id, d.m_vt_id);

                //m_pmdata[nbPMData] = pmData;
                pmdata.push_back(pmData);

                nbPMData++;
            }
            delete[](m_quadrics);
            m_quadrics = nullptr;

            LOG(logINFO) << "Collapsing done";

            return pmdata;
        }

        //--------------------------------------------------

        void ProgressiveMesh::vsplit(ProgressiveMeshData pmData)
        {
            HalfEdge_ptr he = m_dcel->m_halfedge[pmData.getHeFlId()];
            if (he->Twin() == NULL)
                m_nb_faces += 1;
            else
                m_nb_faces += 2;
            m_nb_vertices += 1;

            //LOG(logINFO) << "Vertex Split " << pmData.getVsId() << ", " << pmData.getVtId() << ", faces " << pmData.getFlId() << ", " << pmData.getFrId();

            DcelOperations::vertexSplit(*m_dcel, pmData);
        }

        void ProgressiveMesh::ecol(ProgressiveMeshData pmData)
        {
            HalfEdge_ptr he = m_dcel->m_halfedge[pmData.getHeFlId()];
            if (he->Twin() == NULL)
                m_nb_faces -= 1;
            else
                m_nb_faces -= 2;
            m_nb_vertices -= 1;

            //LOG(logINFO) << "Edge Collapse " << pmData.getVsId() << ", " << pmData.getVtId() << ", faces " << pmData.getFlId() << ", " << pmData.getFrId();

            DcelOperations::edgeCollapse(*m_dcel, pmData);
        }
    }
}