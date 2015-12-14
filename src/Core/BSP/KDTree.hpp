#ifndef RADIUMENGINE_CORE_KDTREE_HPP_
#define RADIUMENGINE_CORE_KDTREE_HPP_
#include <Core/RaCore.hpp>
#include <Core/Containers/VectorArray.hpp>
#include <Core/Math/LinearAlgebra.hpp>
#include <Core/BSP/BSP.hpp>

#include <Core/Containers/AlignedStdVector.hpp>

namespace Ra
{
    namespace Core
    {
        constexpr int PRIMS_PER_LEAF = 8;

        class KDNode
        {
        public:
            RA_CORE_ALIGNED_NEW

            KDNode(int depth = 0)
                :m_is_leaf(true), m_depth(depth), m_aabb(), m_nb_points(0) {}

            KDNode(const KDNode & node)
                :m_is_leaf(node.m_is_leaf), m_depth(node.m_depth), m_nb_points(0) {}

            ~KDNode() {
                // FIX ME
                /*if (!m_points.empty())
                    m_points.clear();*/
            }

            int depth() const { return m_depth; }

            void setIsNotLeaf() { m_is_leaf = false ; }

            void addPrimitive(const Vector4 & data)
            {
                m_points[m_nb_points++] = data.head<3>();
            }

            //void addPrimitives(std::vector<Vector3> data) { m_points.insert(m_points.end(), data.begin(), data.end()) ; }

        public:
            Aabb m_aabb;
        private:
            bool m_is_leaf;
            int m_depth;
            // FIX ME : either a dynamic array or give a size based on KDTree's m_prims_per_leaf (STL structures are crashing, maybe because of Eigen
            Vector3 m_points[PRIMS_PER_LEAF];
            int m_nb_points ;
        };

        class KDTree : public BSP
        {
        public:

            KDTree(int estimated_size = 32)
                :m_nodes()
            {
                m_nodes.reserve(estimated_size);
            }

            ~KDTree()
            {
                clear();
            }

            bool build(const VectorArray<Vector4> & data) override;

            void clear() override;

        protected:
            std::vector<KDNode> m_nodes;
            int m_dim = 3;
            int m_prims_per_leaf = PRIMS_PER_LEAF;
        };
    }
}

#include <Core/BSP/KDTree.inl>

#endif // RADIUMENGINE_CORE_KDTREE_HPP_
