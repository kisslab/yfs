// the extent server implementation

#include "extent_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "zdebug.h"
extent_server::extent_server() {
  int r;
  put(1, "", r);
}


int extent_server::put(extent_protocol::extentid_t id, std::string buf, int &)
{
  // You fill this in for Lab 2.
  Z("extent_server put : %llx %s\n", id, buf.c_str());
  extent_protocol::attr a;
  a.mtime = a.ctime = a.atime = time(NULL);
  a.size = buf.length();
  if (exts.find(id) != exts.end()) {
    a.atime = exts[id].attribute.atime;
  }
  exts[id].name = buf;
  exts[id].attribute = a;
  return extent_protocol::OK;
}

int extent_server::get(extent_protocol::extentid_t id, std::string &buf)
{
  // You fill this in for Lab 2.
  Z("extent_server get : %llx\n", id);
  if (exts.find(id) == exts.end()) {
    return extent_protocol::NOENT;
  }
  buf = exts[id].name;
  exts[id].attribute.atime = time(NULL);
  return extent_protocol::OK;
}

int extent_server::getattr(extent_protocol::extentid_t id, extent_protocol::attr &a)
{
  // You fill this in for Lab 2.
  // You replace this with a real implementation. We send a phony response
  // for now because it's difficult to get FUSE to do anything (including
  // unmount) if getattr fails.
  //a.size = 0;
  //a.atime = 0;
  //a.mtime = 0;
  //a.ctime = 0;
  Z("extent_server getattr : %llx \n", id);
  if (exts.find(id) == exts.end()) {
      return extent_protocol::NOENT;
  }
  a = exts[id].attribute;
  return extent_protocol::OK;
}

int extent_server::remove(extent_protocol::extentid_t id, int &)
{
  // You fill this in for Lab 2.
  Z("extent_server remove : %llx \n", id);
  std::map<extent_protocol::extentid_t, Extent>::iterator it;
  it = exts.find(id);
  if (it == exts.end()) {
    return extent_protocol::NOENT;
  }
  exts.erase(it);
  return extent_protocol::OK;
}

