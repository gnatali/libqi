/*
**  Copyright (C) 2012 Aldebaran Robotics
**  See COPYING for the license
*/
#include <cstring>

#include <qi/bufferreader.hpp>
#include "buffer_p.hpp"
namespace qi {

  BufferReader::BufferReader(const Buffer& buf)
  : _cursor(0)
  , _subCursor(0)
  {
    if (!buf._p)
      const_cast<Buffer&>(buf)._p = boost::shared_ptr<BufferPrivate>(new BufferPrivate());
    _buffer = buf;
  }

  BufferReader::~BufferReader()
  {
  }

  size_t BufferReader::seek(long offset)
  {
    if (_cursor + offset <= _buffer._p->used)
    {
      _cursor += offset;
      return _cursor;
    }
    else
    {
      return -1;
    }
  }

  void *BufferReader::peek(size_t size) const
  {
    if (_cursor + size <= _buffer._p->used)
      return _cursor + _buffer._p->data();
    else
      return 0;
  }

  void *BufferReader::read(size_t size)
  {
    void *p = 0;
    if ((p = peek(size)))
      seek(size);
    else
      return 0;

    return p;
  }

  size_t BufferReader::read(void *data, size_t size)
  {
    if (_buffer._p->used - _cursor < size)
    {
      size = _buffer._p->used - _cursor;
    }
    memcpy(data, _buffer._p->data() + _cursor, size);
    _cursor += size;

    return size;
  }

  bool BufferReader::hasSubBuffer() const
  {
    if (_buffer.subBuffers().size() <= _subCursor)
      return false;
    if (_buffer.subBuffers()[_subCursor].first != _cursor)
      qiLogWarning("BufferReader") << "subBuffer offset mismatch";
    return true;
  }

  Buffer BufferReader::subBuffer()
  {
    if (!hasSubBuffer())
      return Buffer();
    return _buffer.subBuffers()[_subCursor++].second;
  }

}
