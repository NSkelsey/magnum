/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Query.h"

namespace Magnum {

AbstractQuery::AbstractQuery() {
    /** @todo Get some extension wrangler instead to avoid undeclared glGenQueries() on ES2 */
    #ifndef MAGNUM_TARGET_GLES2
    glGenQueries(1, &_id);
    #endif
}

AbstractQuery::~AbstractQuery() {
    /** @todo Get some extension wrangler instead to avoid undeclared glGenQueries() on ES2 */
    #ifndef MAGNUM_TARGET_GLES2
    glDeleteQueries(1, &_id);
    #endif
}

bool AbstractQuery::resultAvailable() {
    /** @todo Re-enable when extension wrangler is available for ES */
    #ifndef MAGNUM_TARGET_GLES2
    GLuint result;
    glGetQueryObjectuiv(_id, GL_QUERY_RESULT_AVAILABLE, &result);
    return result == GL_TRUE;
    #else
    return false;
    #endif
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template<> bool AbstractQuery::result<bool>() {
    /** @todo Re-enable when extension wrangler is available for ES */
    #ifndef MAGNUM_TARGET_GLES2
    GLuint result;
    glGetQueryObjectuiv(_id, GL_QUERY_RESULT, &result);
    return result == GL_TRUE;
    #else
    return false;
    #endif
}

template<> UnsignedInt AbstractQuery::result<UnsignedInt>() {
    /** @todo Re-enable when extension wrangler is available for ES */
    #ifndef MAGNUM_TARGET_GLES2
    UnsignedInt result;
    glGetQueryObjectuiv(_id, GL_QUERY_RESULT, &result);
    return result;
    #else
    return 0;
    #endif
}

#ifndef MAGNUM_TARGET_GLES
template<> Int AbstractQuery::result<Int>() {
    Int result;
    glGetQueryObjectiv(_id, GL_QUERY_RESULT, &result);
    return result;
}

template<> UnsignedLong AbstractQuery::result<UnsignedLong>() {
    UnsignedLong result;
    glGetQueryObjectui64v(_id, GL_QUERY_RESULT, &result);
    return result;
}

template<> Long AbstractQuery::result<Long>() {
    Long result;
    glGetQueryObjecti64v(_id, GL_QUERY_RESULT, &result);
    return result;
}
#endif
#endif

#ifndef MAGNUM_TARGET_GLES2
Query::Query(): target(nullptr) {}

Query::~Query() { delete target; }

void Query::begin(Query::Target target) {
    glBeginQuery(static_cast<GLenum>(target), id());
    this->target = new Target(target);
}

void Query::end() {
    if(!target) return;

    glEndQuery(static_cast<GLenum>(*target));
    delete target;
    target = nullptr;
}
#endif

SampleQuery::SampleQuery(): target(nullptr) {}

SampleQuery::~SampleQuery() { delete target; }

void SampleQuery::begin(SampleQuery::Target target) {
    /** @todo Re-enable when extension wrangler is available for ES */
    #ifndef MAGNUM_TARGET_GLES2
    glBeginQuery(static_cast<GLenum>(target), id());
    #endif
    this->target = new Target(target);
}

void SampleQuery::end() {
    if(!target) return;

    /** @todo Re-enable when extension wrangler is available for ES */
    #ifndef MAGNUM_TARGET_GLES2
    glEndQuery(static_cast<GLenum>(*target));
    #endif
    delete target;
    target = nullptr;
}

#ifndef MAGNUM_TARGET_GLES
TimeQuery::TimeQuery() = default;
#endif

}
