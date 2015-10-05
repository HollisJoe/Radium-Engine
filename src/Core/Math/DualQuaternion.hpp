#ifndef RADIUMENGINE_DUALQUATERNION_HPP
#define RADIUMENGINE_DUALQUATERNION_HPP

#include <Core/RaCore.hpp>
#include <Core/CoreMacros.hpp>
#include <Core/Math/LinearAlgebra.hpp>

namespace Ra
{
    namespace Core
    {
        /// Dual quaternions are based on the dual-numbers algebra, somewhat
        /// analogous to complex numbers, but with the imaginary unit `e` defined
        /// such as e*e = 0 ; and using quaternions as the non-dual and dual part.
        /// Unit dual quaternions can represent any rigid transformation
        /// (rotation + translation).

        /// A good reference.
        /// http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/other/dualQuaternion/index.htm

        class DualQuaternion
        {

        public:
            RA_CORE_ALIGNED_NEW

            // Construct an uninitialized dual quaternion.
            inline DualQuaternion() {}

            /// Construct a dual-quaternion from two quaternions.
            inline DualQuaternion( const Quaternion& q0, const Quaternion& qe ) : m_q0( q0 ), m_qe( qe ) { }

            /// Construct a dual-quaternion from a rigid transform 
            /// Any non-rigid component (e.g. scale and shear) will be ignored.
            DualQuaternion( const Core::Transform& tr );

            /// Default copy constructor and assignment operator.
            DualQuaternion( const DualQuaternion& other ) = default;
            DualQuaternion& operator= ( const DualQuaternion& ) = default;


            /// Getters and setters
            inline const Quaternion& getQ0() const;
            inline void setQ0( const Quaternion& q0 );
            inline const Quaternion& getQe() const;
            inline void setQe( const Quaternion& qe );

            /// Operators

            inline DualQuaternion operator+ ( const DualQuaternion& other );
            inline DualQuaternion operator* ( Scalar scalar );

            inline DualQuaternion& operator += ( const DualQuaternion& other );
            inline DualQuaternion& operator *= ( Scalar scalar );

            /// Other methods

            /// Set the dual-quaternion from a rigid transform.
            /// Any non-rigid component (e.g. scale and shear) will be ignored.
            void setFromTransform( const Transform& t );

            /// Return the corresponding rigid transform. Assume a unit dual quaternion.
            Transform getTransform() const;

            /// Normalize the quaternion with the dual-number norm (divides q0 and qe
            /// by q0's norm).
            inline void normalize();
			
			inline Vector3 transform(const Vector3& p) const;

        private:
            /// non-dual part (representing the rotation)
            Quaternion m_q0;
            /// dual part (representing the translation)
            Quaternion m_qe;
        };
    }
} // namespace Ra::Core

#include <Core/Math/DualQuaternion.inl>

#endif //RADIUMENGINE_DUALQUATERNION_HPP
