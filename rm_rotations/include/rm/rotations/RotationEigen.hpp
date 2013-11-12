/*
 * RotationEigen.hpp
 *
 *  Created on: Oct 28, 2013
 *      Author: remod
 */

#ifndef ROTATIONEIGEN_HPP_
#define ROTATIONEIGEN_HPP_

#include "rm/quaternions/QuaternionEigen.hpp"
#include "RotationBase.hpp"
#include "RotationEigenFunctions.hpp"

namespace rm {
namespace rotations {

namespace eigen_implementation {


template<typename PrimType>
class AngleAxis : public AngleAxisBase<AngleAxis<PrimType>>, private Eigen::AngleAxis<PrimType> {
 private:
  typedef Eigen::AngleAxis<PrimType> Base;
 public:
  typedef Base Implementation;
  typedef PrimType Scalar;

  AngleAxis()
    : Base(Base::Identity()) {
  }

  AngleAxis(const PrimType & chi, const PrimType & v1, const PrimType & v2, const PrimType & v3)
    : Base(chi,Eigen::Matrix<PrimType,3,1>(v1,v2,v3)) {
  }

  // create from Eigen::AngleAxis
  explicit AngleAxis(const Base & other)
      : Base(other) {
  }

  // create from other rotation
  template<typename DERIVED>
  inline explicit AngleAxis(const Rotation<DERIVED> & other)
      : Base(internal::ConversionTraits<AngleAxis, DERIVED>::convert(static_cast<const DERIVED &>(other))) {
  }

  template<typename OTHER_DERIVED>
  AngleAxis & operator =(const Rotation<OTHER_DERIVED> & other) {
    *this = internal::ConversionTraits<AngleAxis, OTHER_DERIVED>::convert(static_cast<const OTHER_DERIVED &>(other));
    return *this;
  }

  AngleAxis inverse() {
    return AngleAxis(Base::inverse());
  }

  inline Base & toImplementation() {
    return static_cast<Base &>(*this);
  }
  inline const Base & toImplementation() const {
    return static_cast<const Base &>(*this);
  }

  using AngleAxisBase<AngleAxis<PrimType>>::operator*;

  friend std::ostream & operator << (std::ostream & out, const AngleAxis & a) {
    out << a.toImplementation().angle() << ", " << a.toImplementation().axis().transpose();
    return out;
  }
};

typedef AngleAxis<double> AngleAxisD;
typedef AngleAxis<float> AngleAxisF;



template<typename PrimType>
class RotationQuaternion : public RotationQuaternionBase<RotationQuaternion<PrimType>>, public quaternions::eigen_implementation::UnitQuaternion<PrimType> {
 private:
  typedef quaternions::eigen_implementation::UnitQuaternion<PrimType> Base;
 public:
  typedef typename Base::Implementation::Implementation Implementation; // TODO: ok?
  typedef PrimType Scalar;

  RotationQuaternion()
    : Base(Implementation::Identity()) { // todo: difference between base and implementation?
  }

  RotationQuaternion(const PrimType & w, const PrimType & x, const PrimType & y, const PrimType & z)
    : Base(w,x,y,z) {
  }

//  w();
//  wWithRealScalarGreaterOrEqualZero();
//  xWithRealScalarGreaterOrEqualZero();
//  yWithRealScalarGreaterOrEqualZero();

  const quaternions::eigen_implementation::UnitQuaternion<PrimType> & getUnitQuaternion() const;
  quaternions::eigen_implementation::UnitQuaternion<PrimType> getUnitQuaternionWithRealScalarGreaterOrEqualZero() const;

  // create from Quaternion
  explicit RotationQuaternion(const Base & other)
    : Base(other) {
  }

  // create from Eigen::Quaternion
  explicit RotationQuaternion(const Implementation & other)
    : Base(other) {
  }

  // create from other rotation
  template<typename DERIVED>
  inline explicit RotationQuaternion(const Rotation<DERIVED> & other)
    : Base(internal::ConversionTraits<RotationQuaternion, DERIVED>::convert(static_cast<const DERIVED &>(other))) {
  }

  template<typename OTHER_DERIVED>
  RotationQuaternion & operator =(const Rotation<OTHER_DERIVED> & other) {
    *this = internal::ConversionTraits<RotationQuaternion, OTHER_DERIVED>::convert(static_cast<const OTHER_DERIVED &>(other));
    return *this;
  }

  RotationQuaternion inverse() {
    return RotationQuaternion(Base::inverse());
  }

  RotationQuaternion conjugate() {
    return RotationQuaternion(Base::conjugate());
  }

  using Base::toImplementation;

  using RotationQuaternionBase<RotationQuaternion<PrimType>>::operator*;
//  using RotationQuaternionBase<RotationQuaternion<PrimType>>::operator==;

  friend std::ostream & operator << (std::ostream & out, const RotationQuaternion & quat) {
    out << quat.toImplementation().w() << " " << quat.toImplementation().x() << " " << quat.toImplementation().y() << " " << quat.toImplementation().z();
    return out;
  }
};

typedef RotationQuaternion<double> RotationQuaternionD;
typedef RotationQuaternion<float> RotationQuaternionF;


template<typename PrimType>
class RotationMatrix : public RotationMatrixBase<RotationMatrix<PrimType>>, private Eigen::Matrix<PrimType, 3, 3> {
 private:
  typedef Eigen::Matrix<PrimType, 3, 3> Base;
 public:
  typedef Base Implementation;
  typedef PrimType Scalar;

  RotationMatrix()
    : Base(Base::Identity()) {
  }

  RotationMatrix(const PrimType & r11, const PrimType & r12, const PrimType & r13,
                 const PrimType & r21, const PrimType & r22, const PrimType & r23,
                 const PrimType & r31, const PrimType & r32, const PrimType & r33) {
    *this << r11,r12,r13,r21,r22,r23,r31,r32,r33;
//    assert(near(*this * this->inverse(), Base::Identity(), 1E-9), "input is not a rotation matrix"); // todo
  }

  // create from Eigen::Matrix
  explicit RotationMatrix(const Base & other)
      : Base(other) {
  }

  // create from other rotation
  template<typename DERIVED>
  inline explicit RotationMatrix(const Rotation<DERIVED> & other) // TODO not explicit anymore
      : Base(internal::ConversionTraits<RotationMatrix, DERIVED>::convert(static_cast<const DERIVED &>(other))) {
  }

  template<typename OTHER_DERIVED>
  RotationMatrix & operator =(const Rotation<OTHER_DERIVED> & other) {
    *this = internal::ConversionTraits<RotationMatrix, OTHER_DERIVED>::convert(static_cast<const OTHER_DERIVED &>(other)); // todo: ok?
    return *this;
  }

  RotationMatrix inverse() {
    return RotationMatrix(toImplementation().transpose());
  }

  inline Implementation & toImplementation() {
    return static_cast<Implementation &>(*this);
  }
  inline const Implementation & toImplementation() const {
    return static_cast<const Implementation &>(*this);
  }

  using RotationMatrixBase<RotationMatrix<PrimType>>::operator*;
  using RotationMatrixBase<RotationMatrix<PrimType>>::operator==;

  friend std::ostream & operator << (std::ostream & out, const RotationMatrix & R) {
    out << R.toImplementation();
    return out;
  }
};

typedef RotationMatrix<double> RotationMatrixD;
typedef RotationMatrix<float> RotationMatrixF;


template<typename PrimType>
class EulerAnglesRPY : public EulerAnglesRPYBase<EulerAnglesRPY<PrimType>>, private Eigen::Matrix<PrimType, 3, 1> {
 private:
  typedef Eigen::Matrix<PrimType, 3, 1> Base;
 public:
  typedef Base Implementation;
  typedef PrimType Scalar;

  EulerAnglesRPY()
    : Base(Base::Zero()) {
  }

  EulerAnglesRPY(const PrimType & r, const PrimType & p, const PrimType & y)
    : Base(r,p,y) {
  }

  // create from Eigen::Matrix
  explicit EulerAnglesRPY(const Base & other)
      : Base(other) {
  }

  // create from other rotation
  template<typename DERIVED>
  inline explicit EulerAnglesRPY(const Rotation<DERIVED> & other)
      : Base(internal::ConversionTraits<EulerAnglesRPY, DERIVED>::convert(static_cast<const DERIVED &>(other))) {
  }

  template<typename OTHER_DERIVED>
  EulerAnglesRPY & operator =(const Rotation<OTHER_DERIVED> & other) {
    *this = internal::ConversionTraits<EulerAnglesRPY, OTHER_DERIVED>::convert(static_cast<const OTHER_DERIVED &>(other)); // todo: ok?
    return *this;
  }

  EulerAnglesRPY inverse() {
    return (EulerAnglesRPY)getInverseRPY<PrimType, PrimType>(*this);
  }

  inline Base & toImplementation() {
    return static_cast<Base &>(*this);
  }
  inline const Base & toImplementation() const {
    return static_cast<const Base &>(*this);
  }

  using EulerAnglesRPYBase<EulerAnglesRPY<PrimType>>::operator*;
  using EulerAnglesRPYBase<EulerAnglesRPY<PrimType>>::operator==;

  friend std::ostream & operator << (std::ostream & out, const EulerAnglesRPY & rpy) {
    out << rpy.toImplementation().transpose();
    return out;
  }
};

typedef EulerAnglesRPY<double> EulerAnglesRPYD;
typedef EulerAnglesRPY<float> EulerAnglesRPYF;


template<typename PrimType>
class EulerAnglesYPR : public EulerAnglesYPRBase<EulerAnglesYPR<PrimType>>, private Eigen::Matrix<PrimType, 3, 1> {
 private:
  typedef Eigen::Matrix<PrimType, 3, 1> Base;
 public:
  typedef Base Implementation;
  typedef PrimType Scalar;

  EulerAnglesYPR()
    : Base(Base::Zero()) {
  }

  EulerAnglesYPR(const PrimType & y, const PrimType & p, const PrimType & r)
    : Base(y,p,r) {
  }

  // create from Eigen::Matrix
  explicit EulerAnglesYPR(const Base & other)
      : Base(other) {
  }

  // create from other rotation
  template<typename DERIVED>
  inline explicit EulerAnglesYPR(const Rotation<DERIVED> & other)
      : Base(internal::ConversionTraits<EulerAnglesYPR, DERIVED>::convert(static_cast<const DERIVED &>(other))) {
  }

  template<typename OTHER_DERIVED>
  EulerAnglesYPR & operator =(const Rotation<OTHER_DERIVED> & other) {
    *this = internal::ConversionTraits<EulerAnglesYPR, OTHER_DERIVED>::convert(static_cast<const OTHER_DERIVED &>(other)); // todo: ok?
    return *this;
  }

  EulerAnglesYPR inverse() {
    return (EulerAnglesYPR)getInverseYPR<PrimType, PrimType>(*this);
  }

  inline Base & toImplementation() {
    return static_cast<Base &>(*this);
  }
  inline const Base & toImplementation() const {
    return static_cast<const Base &>(*this);
  }

  using EulerAnglesYPRBase<EulerAnglesYPR<PrimType>>::operator*;
  using EulerAnglesYPRBase<EulerAnglesYPR<PrimType>>::operator==;

  friend std::ostream & operator << (std::ostream & out, const EulerAnglesYPR & ypr) {
    out << ypr.toImplementation().transpose();
    return out;
  }
};

typedef EulerAnglesYPR<double> EulerAnglesYPRD;
typedef EulerAnglesYPR<float> EulerAnglesYPRF;



}  // namespace eigen_implementation


namespace internal {

// vector
template<typename PrimType>
class get_vector3<eigen_implementation::AngleAxis<PrimType>>{
 public:
  typedef Eigen::Matrix<PrimType, 3, 1> type;
};

template<typename PrimType>
class get_vector3<eigen_implementation::RotationQuaternion<PrimType>>{
 public:
  typedef Eigen::Matrix<PrimType, 3, 1> type;
};

template<typename PrimType>
class get_vector3<eigen_implementation::RotationMatrix<PrimType>>{
 public:
  typedef Eigen::Matrix<PrimType, 3, 1> type;
};

template<typename PrimType>
class get_vector3<eigen_implementation::EulerAnglesRPY<PrimType>>{
 public:
  typedef Eigen::Matrix<PrimType, 3, 1> type;
};

template<typename PrimType>
class get_vector3<eigen_implementation::EulerAnglesYPR<PrimType>>{
 public:
  typedef Eigen::Matrix<PrimType, 3, 1> type;
};

// matrix
template<typename PrimType>
class get_matrix3X<eigen_implementation::AngleAxis<PrimType>>{
 public:
  typedef typename Eigen::Matrix<PrimType, 1, 1>::Index  IndexType;

  template <IndexType Cols>
  class Matrix3X {
   public:
    typedef Eigen::Matrix<PrimType, 3, Cols> type;
  };
};

template<typename PrimType>
class get_matrix3X<eigen_implementation::RotationQuaternion<PrimType>>{
 public:
  typedef typename Eigen::Matrix<PrimType, 1, 1>::Index IndexType;

  template <IndexType Cols>
  class Matrix3X {
   public:
    typedef Eigen::Matrix<PrimType, 3, Cols> type;
  };
};

template<typename PrimType>
class get_matrix3X<eigen_implementation::RotationMatrix<PrimType>>{
 public:
  typedef typename Eigen::Matrix<PrimType, 1, 1>::Index IndexType;

  template <IndexType Cols>
  class Matrix3X {
   public:
    typedef Eigen::Matrix<PrimType, 3, Cols> type;
  };
};

template<typename PrimType>
class get_matrix3X<eigen_implementation::EulerAnglesRPY<PrimType>>{
 public:
  typedef typename Eigen::Matrix<PrimType, 1, 1>::Index IndexType;

  template <IndexType Cols>
  class Matrix3X {
   public:
    typedef Eigen::Matrix<PrimType, 3, Cols> type;
  };
};

template<typename PrimType>
class get_matrix3X<eigen_implementation::EulerAnglesYPR<PrimType>>{
 public:
  typedef typename Eigen::Matrix<PrimType, 1, 1>::Index IndexType;

  template <IndexType Cols>
  class Matrix3X {
   public:
    typedef Eigen::Matrix<PrimType, 3, Cols> type;
  };
};


template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::AngleAxis<DestPrimType>, eigen_implementation::AngleAxis<SourcePrimType>> {
public:
  inline static eigen_implementation::AngleAxis<DestPrimType> convert(const eigen_implementation::AngleAxis<SourcePrimType> & a) {
    return eigen_implementation::AngleAxis<DestPrimType>(a.toImplementation().template cast<DestPrimType>());
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::AngleAxis<DestPrimType>, eigen_implementation::RotationQuaternion<SourcePrimType>> {
public:
  inline static eigen_implementation::AngleAxis<DestPrimType> convert(const eigen_implementation::RotationQuaternion<SourcePrimType> & q) {
    return eigen_implementation::AngleAxis<DestPrimType>(getAngleAxisFromQuaternion<SourcePrimType, DestPrimType>(q.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::AngleAxis<DestPrimType>, eigen_implementation::RotationMatrix<SourcePrimType>> {
public:
  inline static eigen_implementation::AngleAxis<DestPrimType> convert(const eigen_implementation::RotationMatrix<SourcePrimType> & R) {
    return eigen_implementation::AngleAxis<DestPrimType>(getAngleAxisFromRotationMatrix<SourcePrimType, DestPrimType>(R.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::AngleAxis<DestPrimType>, eigen_implementation::EulerAnglesRPY<SourcePrimType>> {
public:
  inline static eigen_implementation::AngleAxis<DestPrimType> convert(const eigen_implementation::EulerAnglesRPY<SourcePrimType> & rpy) {
    return eigen_implementation::AngleAxis<DestPrimType>(getAngleAxisFromRPY<SourcePrimType, DestPrimType>(rpy.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::AngleAxis<DestPrimType>, eigen_implementation::EulerAnglesYPR<SourcePrimType>> {
public:
  inline static eigen_implementation::AngleAxis<DestPrimType> convert(const eigen_implementation::EulerAnglesYPR<SourcePrimType> & ypr) {
    return eigen_implementation::AngleAxis<DestPrimType>(getAngleAxisFromYPR<SourcePrimType, DestPrimType>(ypr.toImplementation()));
  }
};


template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationQuaternion<DestPrimType>, eigen_implementation::AngleAxis<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationQuaternion<DestPrimType> convert(const eigen_implementation::AngleAxis<SourcePrimType> & aa) {
    return eigen_implementation::RotationQuaternion<DestPrimType>(getQuaternionFromAngleAxis<SourcePrimType, DestPrimType>(aa.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationQuaternion<DestPrimType>, eigen_implementation::RotationQuaternion<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationQuaternion<DestPrimType> convert(const eigen_implementation::RotationQuaternion<SourcePrimType> & q) {
    return eigen_implementation::RotationQuaternion<DestPrimType>(q.toImplementation().template cast<DestPrimType>());
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationQuaternion<DestPrimType>, eigen_implementation::RotationMatrix<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationQuaternion<DestPrimType> convert(const eigen_implementation::RotationMatrix<SourcePrimType> & R) {
    return eigen_implementation::RotationQuaternion<DestPrimType>(getQuaternionFromRotationMatrix<SourcePrimType, DestPrimType>(R.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationQuaternion<DestPrimType>, eigen_implementation::EulerAnglesRPY<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationQuaternion<DestPrimType> convert(const eigen_implementation::EulerAnglesRPY<SourcePrimType> & rpy) {
    return eigen_implementation::RotationQuaternion<DestPrimType>(getQuaternionFromRPY<SourcePrimType, DestPrimType>(rpy.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationQuaternion<DestPrimType>, eigen_implementation::EulerAnglesYPR<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationQuaternion<DestPrimType> convert(const eigen_implementation::EulerAnglesYPR<SourcePrimType> & ypr) {
    return eigen_implementation::RotationQuaternion<DestPrimType>(getQuaternionFromYPR<SourcePrimType, DestPrimType>(ypr.toImplementation()));
  }
};


template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationMatrix<DestPrimType>, eigen_implementation::AngleAxis<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationMatrix<DestPrimType> convert(const eigen_implementation::AngleAxis<SourcePrimType> & aa) {
    return eigen_implementation::RotationMatrix<DestPrimType>(getRotationMatrixFromAngleAxis<SourcePrimType, DestPrimType>(aa.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationMatrix<DestPrimType>, eigen_implementation::RotationQuaternion<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationMatrix<DestPrimType> convert(const eigen_implementation::RotationQuaternion<SourcePrimType> & q) {
    return eigen_implementation::RotationMatrix<DestPrimType>(getRotationMatrixFromQuaternion<SourcePrimType, DestPrimType>(q.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationMatrix<DestPrimType>, eigen_implementation::RotationMatrix<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationMatrix<DestPrimType> convert(const eigen_implementation::RotationMatrix<SourcePrimType> & R) {
    return eigen_implementation::RotationMatrix<DestPrimType>(R.toImplementation().template cast<DestPrimType>());
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationMatrix<DestPrimType>, eigen_implementation::EulerAnglesRPY<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationMatrix<DestPrimType> convert(const eigen_implementation::EulerAnglesRPY<SourcePrimType> & rpy) {
    return eigen_implementation::RotationMatrix<DestPrimType>(getRotationMatrixFromRPY<SourcePrimType, DestPrimType>(rpy.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::RotationMatrix<DestPrimType>, eigen_implementation::EulerAnglesYPR<SourcePrimType>> {
public:
  inline static eigen_implementation::RotationMatrix<DestPrimType> convert(const eigen_implementation::EulerAnglesYPR<SourcePrimType> & ypr) {
    return eigen_implementation::RotationMatrix<DestPrimType>(getRotationMatrixFromYPR<SourcePrimType, DestPrimType>(ypr.toImplementation()));
  }
};


template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesRPY<DestPrimType>, eigen_implementation::AngleAxis<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<DestPrimType> convert(const eigen_implementation::AngleAxis<SourcePrimType> & aa) {
    return eigen_implementation::EulerAnglesRPY<DestPrimType>(getRPYFromAngleAxis<SourcePrimType, DestPrimType>(aa.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesRPY<DestPrimType>, eigen_implementation::RotationQuaternion<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<DestPrimType> convert(const eigen_implementation::RotationQuaternion<SourcePrimType> & q) {
    return eigen_implementation::EulerAnglesRPY<DestPrimType>(getRPYFromQuaternion<SourcePrimType, DestPrimType>(q.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesRPY<DestPrimType>, eigen_implementation::RotationMatrix<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<DestPrimType> convert(const eigen_implementation::RotationMatrix<SourcePrimType> & R) {
    return eigen_implementation::EulerAnglesRPY<DestPrimType>(getRPYFromRotationMatrix<SourcePrimType, DestPrimType>(R.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesRPY<DestPrimType>, eigen_implementation::EulerAnglesRPY<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<DestPrimType> convert(const eigen_implementation::EulerAnglesRPY<SourcePrimType> & rpy) {
    return eigen_implementation::EulerAnglesRPY<DestPrimType>(rpy.toImplementation().template cast<DestPrimType>());
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesRPY<DestPrimType>, eigen_implementation::EulerAnglesYPR<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<DestPrimType> convert(const eigen_implementation::EulerAnglesYPR<SourcePrimType> & ypr) {
    return eigen_implementation::EulerAnglesRPY<DestPrimType>(getRPYFromYPR<SourcePrimType, DestPrimType>(ypr.toImplementation()));
  }
};


template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesYPR<DestPrimType>, eigen_implementation::AngleAxis<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<DestPrimType> convert(const eigen_implementation::AngleAxis<SourcePrimType> & aa) {
    return eigen_implementation::EulerAnglesYPR<DestPrimType>(getYPRFromAngleAxis<SourcePrimType, DestPrimType>(aa.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesYPR<DestPrimType>, eigen_implementation::RotationQuaternion<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<DestPrimType> convert(const eigen_implementation::RotationQuaternion<SourcePrimType> & q) {
    return eigen_implementation::EulerAnglesYPR<DestPrimType>(getYPRFromQuaternion<SourcePrimType, DestPrimType>(q.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesYPR<DestPrimType>, eigen_implementation::RotationMatrix<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<DestPrimType> convert(const eigen_implementation::RotationMatrix<SourcePrimType> & R) {
    return eigen_implementation::EulerAnglesYPR<DestPrimType>(getYPRFromRotationMatrix<SourcePrimType, DestPrimType>(R.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesYPR<DestPrimType>, eigen_implementation::EulerAnglesRPY<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<DestPrimType> convert(const eigen_implementation::EulerAnglesRPY<SourcePrimType> & rpy) {
    return eigen_implementation::EulerAnglesYPR<DestPrimType>(getYPRFromRPY<SourcePrimType, DestPrimType>(rpy.toImplementation()));
  }
};

template<typename DestPrimType, typename SourcePrimType>
class ConversionTraits<eigen_implementation::EulerAnglesYPR<DestPrimType>, eigen_implementation::EulerAnglesYPR<SourcePrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<DestPrimType> convert(const eigen_implementation::EulerAnglesYPR<SourcePrimType> & ypr) {
    return eigen_implementation::EulerAnglesYPR<DestPrimType>(ypr.toImplementation().template cast<DestPrimType>());
  }
};


template<typename PrimType>
class MultiplicationTraits<eigen_implementation::EulerAnglesRPY<PrimType>, eigen_implementation::EulerAnglesRPY<PrimType>> {
public:
  inline static eigen_implementation::EulerAnglesRPY<PrimType> mult(const eigen_implementation::EulerAnglesRPY<PrimType> & a, const eigen_implementation::EulerAnglesRPY<PrimType> & b) {
    return eigen_implementation::EulerAnglesRPY<PrimType>(eigen_implementation::RotationQuaternion<PrimType>(eigen_implementation::RotationQuaternion<PrimType>(a).toImplementation()*eigen_implementation::RotationQuaternion<PrimType>(b).toImplementation()));
  }
};

template<typename PrimType>
class MultiplicationTraits<eigen_implementation::EulerAnglesYPR<PrimType>, eigen_implementation::EulerAnglesYPR<PrimType>> {
public:
  inline static eigen_implementation::EulerAnglesYPR<PrimType> mult(const eigen_implementation::EulerAnglesYPR<PrimType> & a, const eigen_implementation::EulerAnglesYPR<PrimType> & b) {
    return eigen_implementation::EulerAnglesYPR<PrimType>(eigen_implementation::RotationQuaternion<PrimType>(eigen_implementation::RotationQuaternion<PrimType>(a).toImplementation()*eigen_implementation::RotationQuaternion<PrimType>(b).toImplementation()));
  }
};

//// vector
//template<typename PrimType>
//class RotationTraits<eigen_implementation::AngleAxis<PrimType>> {
// public:
//   inline static typename get_vector3<eigen_implementation::AngleAxis<PrimType>>::type rotate(const eigen_implementation::AngleAxis<PrimType> & aa, const typename get_vector3<eigen_implementation::AngleAxis<PrimType>>::type & v){
//     return aa.toImplementation() * v;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::RotationQuaternion<PrimType>> {
// public:
//   inline static typename get_vector3<eigen_implementation::RotationQuaternion<PrimType>>::type rotate(const eigen_implementation::RotationQuaternion<PrimType> & p, const typename get_vector3<eigen_implementation::RotationQuaternion<PrimType>>::type & v){
//     return p.toImplementation() * v;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::RotationMatrix<PrimType>> {
// public:
//   inline static typename get_vector3<eigen_implementation::RotationMatrix<PrimType>>::type rotate(const eigen_implementation::RotationMatrix<PrimType> & R, const typename get_vector3<eigen_implementation::RotationMatrix<PrimType>>::type & v){
//     return R.toImplementation() * v;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::EulerAnglesRPY<PrimType>> {
// public:
//   inline static typename get_vector3<eigen_implementation::EulerAnglesRPY<PrimType>>::type rotate(const eigen_implementation::EulerAnglesRPY<PrimType> & rpy, const typename get_vector3<eigen_implementation::EulerAnglesRPY<PrimType>>::type & v){
//     return eigen_implementation::RotationMatrix<PrimType>(rpy).toImplementation() * v;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::EulerAnglesYPR<PrimType>> {
// public:
//   inline static typename get_vector3<eigen_implementation::EulerAnglesYPR<PrimType>>::type rotate(const eigen_implementation::EulerAnglesYPR<PrimType> & ypr, const typename get_vector3<eigen_implementation::EulerAnglesYPR<PrimType>>::type & v){
//     return eigen_implementation::RotationMatrix<PrimType>(ypr).toImplementation() * v;
//   }
//};

//// matrix
//template<typename PrimType>
//class RotationTraits<eigen_implementation::AngleAxis<PrimType>> {
// public:
//   inline static typename Eigen::Matrix<PrimType, 3, Eigen::Dynamic> rotate(const eigen_implementation::AngleAxis<PrimType> & aa, const Eigen::Matrix<PrimType, 3, Eigen::Dynamic> & m){
//     return aa.toImplementation() * m;
//   }
//};
//
template<typename PrimType>
class RotationTraits<eigen_implementation::RotationQuaternion<PrimType>> {
 public:
  template<typename get_matrix3X<eigen_implementation::RotationQuaternion<PrimType>>::IndexType Cols>
   inline static typename get_matrix3X<eigen_implementation::RotationQuaternion<PrimType>>::template Matrix3X<Cols>::type rotate(const eigen_implementation::RotationQuaternion<PrimType> & p, const typename get_matrix3X<eigen_implementation::RotationQuaternion<PrimType>>::template Matrix3X<Cols>::type & m){
    std::cout << "here" << std::endl;
    return p.toImplementation() * m;
   }
};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::RotationMatrix<PrimType>> {
// public:
//   inline static typename Eigen::Matrix<PrimType, 3, Eigen::Dynamic> rotate(const eigen_implementation::RotationMatrix<PrimType> & R, const Eigen::Matrix<PrimType, 3, Eigen::Dynamic> & m){
//     return R.toImplementation() * m;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::EulerAnglesRPY<PrimType>> {
// public:
//   inline static typename Eigen::Matrix<PrimType, 3, Eigen::Dynamic> rotate(const eigen_implementation::EulerAnglesRPY<PrimType> & rpy, const Eigen::Matrix<PrimType, 3, Eigen::Dynamic> & m){
//     return eigen_implementation::RotationMatrix<PrimType>(rpy).toImplementation() * m;
//   }
//};
//
//template<typename PrimType>
//class RotationTraits<eigen_implementation::EulerAnglesYPR<PrimType>> {
// public:
//   inline static typename Eigen::Matrix<PrimType, 3, Eigen::Dynamic> rotate(const eigen_implementation::EulerAnglesYPR<PrimType> & ypr, const Eigen::Matrix<PrimType, 3, Eigen::Dynamic> & m){
//     return eigen_implementation::RotationMatrix<PrimType>(ypr).toImplementation() * m;
//   }
//};



template<typename PrimType>
class ComparisonTraits<eigen_implementation::AngleAxis<PrimType>> {
 public:
   inline static bool isequal(const eigen_implementation::AngleAxis<PrimType> & a, const eigen_implementation::AngleAxis<PrimType> & b){
     return (a.toImplementation().angle() ==  b.toImplementation().angle() && a.toImplementation().axis() ==  b.toImplementation().axis()) ||
            (a.toImplementation().angle() == -b.toImplementation().angle() && a.toImplementation().axis() == -b.toImplementation().axis());
   }
};

template<typename PrimType>
class ComparisonTraits<eigen_implementation::RotationQuaternion<PrimType>> {
 public:
   inline static bool isequal(const eigen_implementation::RotationQuaternion<PrimType> & a, const eigen_implementation::RotationQuaternion<PrimType> & b){
     return (a.toImplementation().w() ==  b.toImplementation().w() &&
             a.toImplementation().x() ==  b.toImplementation().x() &&
             a.toImplementation().y() ==  b.toImplementation().y() &&
             a.toImplementation().z() ==  b.toImplementation().z()) ||
            (a.toImplementation().w() == -b.toImplementation().w() &&
             a.toImplementation().x() == -b.toImplementation().x() &&
             a.toImplementation().y() == -b.toImplementation().y() &&
             a.toImplementation().z() == -b.toImplementation().z());
   }
};

template<typename PrimType>
class ComparisonTraits<eigen_implementation::RotationMatrix<PrimType>> {
 public:
   inline static bool isequal(const eigen_implementation::RotationMatrix<PrimType> & a, const eigen_implementation::RotationMatrix<PrimType> & b){
     return a.toImplementation() == b.toImplementation();
   }
};

template<typename PrimType>
class ComparisonTraits<eigen_implementation::EulerAnglesRPY<PrimType>> {
 public:
   inline static bool isequal(const eigen_implementation::EulerAnglesRPY<PrimType> & a, const eigen_implementation::EulerAnglesRPY<PrimType> & b){
     return a.toImplementation() == b.toImplementation();
   }
};

template<typename PrimType>
class ComparisonTraits<eigen_implementation::EulerAnglesYPR<PrimType>> {
 public:
   inline static bool isequal(const eigen_implementation::EulerAnglesYPR<PrimType> & a, const eigen_implementation::EulerAnglesYPR<PrimType> & b){
     return a.toImplementation() == b.toImplementation();
   }
};



} // namespace internal
} // namespace rotations
} // namespace rm

#endif /* ROTATIONEIGEN_HPP_ */
