#pragma once

#include "FileIO.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

NAMESPACE {

  template <typename T>
    void load(FILE* file, Vec2<T>* vec) {
    vec->x() = fio::readLittleEndian<T>(file);
    vec->y() = fio::readLittleEndian<T>(file);
  }
  template <typename T>
    void save(FILE* file, Vec2<T>& vec) {
    fio::writeLittleEndian(file, vec.x());
    fio::writeLittleEndian(file, vec.y());
  }

  template <typename T>
    void load(FILE* file, Vec3<T>* vec) {
    vec->x() = fio::readLittleEndian<T>(file);
    vec->y() = fio::readLittleEndian<T>(file);
    vec->z() = fio::readLittleEndian<T>(file);
  }
  template <typename T>
    void save(FILE* file, Vec3<T>& vec) {
    fio::writeLittleEndian(file, vec.x());
    fio::writeLittleEndian(file, vec.y());
    fio::writeLittleEndian(file, vec.z());
  }

  template <typename T>
    void load(FILE* file, Vec4<T>* vec) {
    vec->w() = fio::readLittleEndian<T>(file);
    vec->x() = fio::readLittleEndian<T>(file);
    vec->y() = fio::readLittleEndian<T>(file);
    vec->z() = fio::readLittleEndian<T>(file);
  }
  template <typename T>
    void save(FILE* file, Vec4<T>& vec) {
    fio::writeLittleEndian(file, vec.w());
    fio::writeLittleEndian(file, vec.x());
    fio::writeLittleEndian(file, vec.y());
    fio::writeLittleEndian(file, vec.z());
  }

  template <typename T>
    void load(FILE* file, Quaternion<T>* quat) {
    quat->w() = fio::readLittleEndian<T>(file);
    quat->x() = fio::readLittleEndian<T>(file);
    quat->y() = fio::readLittleEndian<T>(file);
    quat->z() = fio::readLittleEndian<T>(file);
  }
  template <typename T>
    void save(FILE* file, Quaternion<T>& quat) {
    fio::writeLittleEndian(file, quat.x());
    fio::writeLittleEndian(file, quat.y());
    fio::writeLittleEndian(file, quat.z());
    fio::writeLittleEndian(file, quat.w());
  }
  
}
