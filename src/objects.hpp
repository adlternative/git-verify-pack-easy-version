#ifndef ADL_OBJECTS_H__
#define ADL_OBJECTS_H__

#include <cstring>
#include <string>

#define GIT_SHA1_RAWSZ 20

enum object_type {
  OBJ_BAD = -1,
  OBJ_NONE = 0,
  OBJ_COMMIT = 1,
  OBJ_TREE = 2,
  OBJ_BLOB = 3,
  OBJ_TAG = 4,
  /* 5 for future expansion */
  // OBJ_OFS_DELTA = 6,
  // OBJ_REF_DELTA = 7,
  // OBJ_ANY,
  // OBJ_MAX
};

inline std::string get_object_type(enum object_type type) {
  switch (type) {
  case OBJ_BLOB:
    return "blob";
  case OBJ_TREE:
    return "tree";
  case OBJ_COMMIT:
    return "commit";
  case OBJ_TAG:
    return "tag";
  default:
    return "(unknown)";
  }
}

struct object {
  object_type type;
  size_t size;
  uint32_t crc;
  unsigned char oid[GIT_SHA1_RAWSZ];

  void set_oid(const unsigned char objname[]) {
    memcpy(oid, objname, sizeof(oid));
  }

  object() : type(OBJ_BAD), size(0), crc(0) {}

  object(const object &obj) : type(obj.type), size(obj.size), crc(obj.crc) {
    memcpy(oid, obj.oid, sizeof(oid));
  }

  object(object &&obj) noexcept : type(obj.type), size(obj.size), crc(obj.crc) {
    memcpy(oid, obj.oid, sizeof(oid));
  }

  object &operator=(const object &obj) {
    if (&obj != this) {
      type = obj.type;
      size = obj.size;
      crc = obj.crc;
      memcpy(oid, obj.oid, sizeof(oid));
    }
    return *this;
  }
  object &operator=(object &&obj) noexcept {
    if (&obj != this) {
      type = obj.type;
      size = obj.size;
      crc = obj.crc;
      memcpy(oid, obj.oid, sizeof(oid));
    }
    return *this;
  }
};

#endif // ADL_OBJECTS_H__