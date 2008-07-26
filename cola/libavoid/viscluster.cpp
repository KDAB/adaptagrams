/*
 * vim: ts=4 sw=4 et tw=0 wm=0
 *
 * libavoid - Fast, Incremental, Object-avoiding Line Router
 *
 * Copyright (C) 2004-2007  Michael Wybrow <mjwybrow@users.sourceforge.net>
 * Copyright (C) 2008  Monash University
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library in the file LICENSE; if not, 
 * write to the Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307  USA
 * 
*/

#include "libavoid/viscluster.h"
#include "libavoid/router.h"
#include "libavoid/polyutil.h"


namespace Avoid {


ClusterRef::ClusterRef(Router *router, unsigned int id, Polygn& ply)
    : _router(router)
    , _id(id)
    , _poly(ply, router)
    , _active(false)
{
    assert(id > 0);
}


ClusterRef::~ClusterRef()
{
}


void ClusterRef::makeActive(void)
{
    assert(!_active);
    
    // Add to connRefs list.
    _pos = _router->clusterRefs.insert(_router->clusterRefs.begin(), this);

    _active = true;
}


void ClusterRef::makeInactive(void)
{
    assert(_active);
    
    // Remove from connRefs list.
    _router->clusterRefs.erase(_pos);

    _active = false;
}
    

void ClusterRef::setNewPoly(Polygn& poly)
{
    _poly = ReferencingPolygn(poly, _router);
}


unsigned int ClusterRef::id(void)
{
    return _id;
}


ReferencingPolygn& ClusterRef::poly(void)
{
    return _poly;
}


Router *ClusterRef::router(void)
{
    return _router;
}


}


