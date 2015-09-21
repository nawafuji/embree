// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "instance_intersector8.h"

namespace embree
{
  namespace isa
  { 
    void FastInstanceIntersector8::intersect(vbool8* valid, const Instance* instance, Ray8& ray, size_t item)
    {
      const Vec3vf8 ray_org = ray.org;
      const Vec3vf8 ray_dir = ray.dir;
      const vint8 ray_geomID = ray.geomID;
      const vint8 ray_instID = ray.instID;
      const AffineSpace3vf8 world2local(instance->world2local);
      ray.org = xfmPoint (world2local,ray_org);
      ray.dir = xfmVector(world2local,ray_dir);
      ray.geomID = -1;
      ray.instID = instance->id;
      instance->object->intersect8(valid,(RTCRay8&)ray);
      ray.org = ray_org;
      ray.dir = ray_dir;
      vbool8 nohit = ray.geomID == vint8(-1);
      ray.geomID = select(nohit,ray_geomID,ray.geomID);
      ray.instID = select(nohit,ray_instID,ray.instID);
    }
    
    void FastInstanceIntersector8::occluded (vbool8* valid, const Instance* instance, Ray8& ray, size_t item)
    {
      const Vec3vf8 ray_org = ray.org;
      const Vec3vf8 ray_dir = ray.dir;
      const vint8 ray_geomID = ray.geomID;
      const AffineSpace3vf8 world2local(instance->world2local);
      ray.org = xfmPoint (world2local,ray_org);
      ray.dir = xfmVector(world2local,ray_dir);
      ray.instID = instance->id;
      instance->object->occluded8(valid,(RTCRay8&)ray);
      ray.org = ray_org;
      ray.dir = ray_dir;
    }

    DEFINE_SET_INTERSECTOR8(InstanceIntersector8,FastInstanceIntersector8);
  }
}
