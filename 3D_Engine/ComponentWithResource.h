#ifndef __COMPONENT_WITH_RESOURCE_H__
#define __COMPONENT_WITH_RESOURCE_H__

#include "Globals.h"

class Resource;

class ComponentWithResource
{
public:
	ComponentWithResource();
	~ComponentWithResource();

	virtual void SetResource(uuid resource) {};

	virtual Resource* GetResource() const;
	uuid GetResourceUUID()const { return resourceUUID; }

	protected:
	uuid resourceUUID = 0;
};

#endif //!__COMPONENT_WITH_RESOURCE_H__