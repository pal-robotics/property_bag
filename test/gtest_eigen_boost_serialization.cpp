#include "utils_gtest.h"

#include "property_bag/serialization/eigen_boost_serialization.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

TEST(EigenSerializationTest, EigenMatrixBoostSerialization)
{
  std::stringstream ss;

  Eigen::Matrix<double, 10, 10> mat_double = Eigen::Matrix<double, 10, 10>::Random();
  Eigen::Matrix<float, 10, 10> mat_float   = Eigen::Matrix<float, 10, 10>::Random();
  Eigen::Matrix<int, 10, 10> mat_int       = Eigen::Matrix<int, 10, 10>::Random();

  {
    boost::archive::text_oarchive oa(ss);

    ASSERT_NO_THROW(oa << mat_double);

    ASSERT_NO_THROW(oa << mat_float);

    ASSERT_NO_THROW(oa << mat_int);
  }

  PRINTF("EigenSerializationTest::EigenMatrixBoostSerialization Saved !\n");

  {
    boost::archive::text_iarchive ia(ss);

    Eigen::Matrix<double, 10, 10> mat_double_loaded;
    ASSERT_NO_THROW(ia >> mat_double_loaded);
    ASSERT_EQ(mat_double, mat_double_loaded);

    Eigen::Matrix<float, 10, 10> mat_float_loaded;
    ASSERT_NO_THROW(ia >> mat_float_loaded);
    ASSERT_EQ(mat_float, mat_float_loaded);

    Eigen::Matrix<int, 10, 10> mat_int_loaded;
    ASSERT_NO_THROW(ia >> mat_int_loaded);
    ASSERT_EQ(mat_int, mat_int_loaded);
  }

  PRINTF("All good at EigenSerializationTest::EigenMatrixBoostSerialization !\n");
}

TEST(EigenSerializationTest, EigenTripletBoostSerialization)
{
  std::stringstream ss;

  Eigen::Triplet<double> mat_double(0,1,2);
  Eigen::Triplet<float> mat_float(0,1,2);
  Eigen::Triplet<int> mat_int(0,1,2);

  {
    boost::archive::text_oarchive oa(ss);

    ASSERT_NO_THROW(oa << mat_double);

    ASSERT_NO_THROW(oa << mat_float);

    ASSERT_NO_THROW(oa << mat_int);
  }

  PRINTF("EigenSerializationTest::EigenTripletBoostSerialization Saved !\n");

  {
    boost::archive::text_iarchive ia(ss);

    Eigen::Triplet<double> mat_double_loaded;
    ASSERT_NO_THROW(ia >> mat_double_loaded);
    ASSERT_EQ(mat_double.col(), mat_double_loaded.col());
    ASSERT_EQ(mat_double.row(), mat_double_loaded.row());
    ASSERT_EQ(mat_double.value(), mat_double_loaded.value());

    Eigen::Triplet<float> mat_float_loaded;
    ASSERT_NO_THROW(ia >> mat_float_loaded);
    ASSERT_EQ(mat_float.col(), mat_float_loaded.col());
    ASSERT_EQ(mat_float.row(), mat_float_loaded.row());
    ASSERT_EQ(mat_float.value(), mat_float_loaded.value());

    Eigen::Triplet<int> mat_int_loaded;
    ASSERT_NO_THROW(ia >> mat_int_loaded);
    ASSERT_EQ(mat_int.col(), mat_int_loaded.col());
    ASSERT_EQ(mat_int.row(), mat_int_loaded.row());
    ASSERT_EQ(mat_int.value(), mat_int_loaded.value());
  }

  PRINTF("All good at EigenSerializationTest::EigenTripletBoostSerialization !\n");
}

///@todo missing == operator
//TEST(EigenSerializationTest, EigenSparseMatrixBoostSerialization)
//{
//  std::stringstream ss;

//  int rows=100;
//  int cols=100;

//  Eigen::SparseMatrix<double> mat_double(rows, cols);
//  Eigen::SparseMatrix<float> mat_float(rows, cols);
//  Eigen::SparseMatrix<int> mat_int(rows, cols);

//  std::default_random_engine gen;
//  std::uniform_real_distribution<double> dist(0.0,1.0);

//  std::vector<Eigen::Triplet<double>> triplet_double;
//  std::vector<Eigen::Triplet<float>> triplet_float;
//  std::vector<Eigen::Triplet<int>> triplet_int;
//  for(int i=0;i<rows;++i)
//    for(int j=0;j<cols;++j)
//    {
//      auto v_ij=dist(gen); //generate random number
//      if(v_ij < 0.1)
//      {
//        triplet_double.push_back(T(i,j,v_ij)); //if larger than treshold, insert it
//        triplet_float.push_back(T(i,j,v_ij)); //if larger than treshold, insert it
//        triplet_int.push_back(T(i,j,v_ij)); //if larger than treshold, insert it
//      }
//    }

//  mat_double.setFromTriplets(triplet_double.begin(), triplet_double.end());
//  mat_float.setFromTriplets(triplet_float.begin(), triplet_float.end());
//  mat_int.setFromTriplets(triplet_int.begin(), triplet_int.end());

//  {
//    boost::archive::text_oarchive oa(ss);

//    ASSERT_NO_THROW(oa << mat_double);

//    ASSERT_NO_THROW(oa << mat_float);

//    ASSERT_NO_THROW(oa << mat_int);
//  }

//  PRINTF("EigenSerializationTest::EigenSparseMatrixBoostSerialization Saved !\n");

//  {
//    boost::archive::text_iarchive ia(ss);

//    Eigen::SparseMatrix<double> mat_double_loaded;
//    ASSERT_NO_THROW(ia >> mat_double_loaded);
//    ASSERT_EQ(mat_double, mat_double_loaded);

//    Eigen::SparseMatrix<float> mat_float_loaded;
//    ASSERT_NO_THROW(ia >> mat_float_loaded);
//    ASSERT_EQ(mat_float, mat_float_loaded);

//    Eigen::SparseMatrix<int> mat_int_loaded;
//    ASSERT_NO_THROW(ia >> mat_int_loaded);
//    ASSERT_EQ(mat_int, mat_int_loaded);
//  }

//  PRINTF("All good at EigenSerializationTest::EigenSparseMatrixBoostSerialization !\n");
//}

TEST(EigenSerializationTest, EigenQuaternionBoostSerialization)
{
  std::stringstream ss;

  Eigen::Quaternion<double> q_double(0,0,0,1);
  Eigen::Quaternion<float> q_float = Eigen::Quaternion<float>(4,3,2,1).normalized();

  {
    boost::archive::text_oarchive oa(ss);

    ASSERT_NO_THROW(oa << q_double);

    ASSERT_NO_THROW(oa << q_float);
  }

  PRINTF("EigenSerializationTest::EigenQuaternionBoostSerialization Saved !\n");

  {
    boost::archive::text_iarchive ia(ss);

    Eigen::Quaternion<double> q_double_loaded;
    ASSERT_NO_THROW(ia >> q_double_loaded);
    ASSERT_EQ(q_double.x(), q_double_loaded.x());
    ASSERT_EQ(q_double.y(), q_double_loaded.y());
    ASSERT_EQ(q_double.z(), q_double_loaded.z());
    ASSERT_EQ(q_double.w(), q_double_loaded.w());

    Eigen::Quaternion<float> q_float_loaded;
    ASSERT_NO_THROW(ia >> q_float_loaded);
    ASSERT_EQ(q_float.x(), q_float_loaded.x());
    ASSERT_EQ(q_float.y(), q_float_loaded.y());
    ASSERT_EQ(q_float.z(), q_float_loaded.z());
    ASSERT_EQ(q_float.w(), q_float_loaded.w());
  }

  PRINTF("All good at EigenSerializationTest::EigenQuaternionBoostSerialization !\n");
}

TEST(EigenSerializationTest, EigenTransformBoostSerialization)
{
  std::stringstream ss;

  ///@todo there is no 'Random' and I'm lazy so -> Identity.
  Eigen::Isometry2f iso2f = Eigen::Isometry2f::Identity();
  Eigen::Isometry2d iso2d = Eigen::Isometry2d::Identity();
  Eigen::Isometry3f iso3f = Eigen::Isometry3f::Identity();
  Eigen::Isometry3d iso3d = Eigen::Isometry3d::Identity();

  Eigen::Affine2f aff2f = Eigen::Affine2f::Identity();
  Eigen::Affine2d aff2d = Eigen::Affine2d::Identity();
  Eigen::Affine3f aff3f = Eigen::Affine3f::Identity();
  Eigen::Affine3d aff3d = Eigen::Affine3d::Identity();

  Eigen::AffineCompact2f affc2f = Eigen::AffineCompact2f::Identity();
  Eigen::AffineCompact2d affc2d = Eigen::AffineCompact2d::Identity();
  Eigen::AffineCompact3f affc3f = Eigen::AffineCompact3f::Identity();
  Eigen::AffineCompact3d affc3d = Eigen::AffineCompact3d::Identity();

  {
    boost::archive::text_oarchive oa(ss);

    ASSERT_NO_THROW(oa << iso2f);
    ASSERT_NO_THROW(oa << iso2d);
    ASSERT_NO_THROW(oa << iso3f);
    ASSERT_NO_THROW(oa << iso3d);

    ASSERT_NO_THROW(oa << aff2f);
    ASSERT_NO_THROW(oa << aff2d);
    ASSERT_NO_THROW(oa << aff3f);
    ASSERT_NO_THROW(oa << aff3d);

    ASSERT_NO_THROW(oa << affc2f);
    ASSERT_NO_THROW(oa << affc2d);
    ASSERT_NO_THROW(oa << affc3f);
    ASSERT_NO_THROW(oa << affc3d);
  }

  PRINTF("EigenSerializationTest::EigenTransformBoostSerialization Saved !\n");

  {
    boost::archive::text_iarchive ia(ss);

    Eigen::Isometry2f iso2f_l;
    Eigen::Isometry2d iso2d_l;
    Eigen::Isometry3f iso3f_l;
    Eigen::Isometry3d iso3d_l;

    Eigen::Affine2f aff2f_l;
    Eigen::Affine2d aff2d_l;
    Eigen::Affine3f aff3f_l;
    Eigen::Affine3d aff3d_l;

    Eigen::AffineCompact2f affc2f_l;
    Eigen::AffineCompact2d affc2d_l;
    Eigen::AffineCompact3f affc3f_l;
    Eigen::AffineCompact3d affc3d_l;

    ASSERT_NO_THROW(ia >> iso2f_l);
    ASSERT_NO_THROW(ia >> iso2d_l);
    ASSERT_NO_THROW(ia >> iso3f_l);
    ASSERT_NO_THROW(ia >> iso3d_l);

    ASSERT_NO_THROW(ia >> aff2f_l);
    ASSERT_NO_THROW(ia >> aff2d_l);
    ASSERT_NO_THROW(ia >> aff3f_l);
    ASSERT_NO_THROW(ia >> aff3d_l);

    ASSERT_NO_THROW(ia >> affc2f_l);
    ASSERT_NO_THROW(ia >> affc2d_l);
    ASSERT_NO_THROW(ia >> affc3f_l);
    ASSERT_NO_THROW(ia >> affc3d_l);
  }

  PRINTF("All good at EigenSerializationTest::EigenTransformBoostSerialization !\n");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
