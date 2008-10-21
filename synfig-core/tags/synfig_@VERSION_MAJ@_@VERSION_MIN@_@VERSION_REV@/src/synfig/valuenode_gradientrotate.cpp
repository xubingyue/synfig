/* === S Y N F I G ========================================================= */
/*!	\file valuenode_gradientrotate.cpp
**	\brief Implementation of the "Gradient Rotate" valuenode conversion.
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007, 2008 Chris Moore
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "general.h"
#include "valuenode_gradientrotate.h"
#include "valuenode_const.h"
#include <stdexcept>
#include "gradient.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

synfig::ValueNode_GradientRotate::ValueNode_GradientRotate(const Gradient& x):
	LinkableValueNode(synfig::ValueBase::TYPE_GRADIENT)
{
	set_link("gradient",ValueNode_Const::create(x));
	set_link("offset",ValueNode_Const::create(Real(0)));
	DCAST_HACK_ENABLE();
}

LinkableValueNode*
ValueNode_GradientRotate::create_new()const
{
	return new ValueNode_GradientRotate(Gradient());
}

ValueNode_GradientRotate*
ValueNode_GradientRotate::create(const ValueBase& x)
{
	ValueBase::Type id(x.get_type());
	if(id!=ValueBase::TYPE_GRADIENT)
	{
		assert(0);
		throw runtime_error(String(_("Gradient Rotate"))+_(":Bad type ")+ValueBase::type_local_name(id));
	}

	ValueNode_GradientRotate* value_node=new ValueNode_GradientRotate(x.get(Gradient()));

	assert(value_node->get_type()==id);

	return value_node;
}

synfig::ValueNode_GradientRotate::~ValueNode_GradientRotate()
{
	unlink_all();
}

synfig::ValueBase
synfig::ValueNode_GradientRotate::operator()(Time t)const
{
	Gradient gradient;
	gradient=(*ref_gradient)(t).get(gradient);
	Real offset((*ref_offset)(t).get(Real()));
	Gradient::iterator iter;
	for(iter=gradient.begin();iter!=gradient.end();++iter)
		iter->pos+=offset;

	return gradient;
}

bool
ValueNode_GradientRotate::set_link_vfunc(int i,ValueNode::Handle value)
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: CHECK_TYPE_AND_SET_VALUE(ref_gradient, ValueBase::TYPE_GRADIENT);
	case 1: CHECK_TYPE_AND_SET_VALUE(ref_offset,   ValueBase::TYPE_REAL);
	}
	return false;
}

ValueNode::LooseHandle
ValueNode_GradientRotate::get_link_vfunc(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0:
			return ref_gradient;
		case 1:
			return ref_offset;
	}
	return 0;
}

int
ValueNode_GradientRotate::link_count()const
{
	return 2;
}

String
ValueNode_GradientRotate::link_local_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0:
			return _("Gradient");
		case 1:
			return _("Offset");
		default:
			return String();
	}
}

String
ValueNode_GradientRotate::link_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0:
			return "gradient";
		case 1:
			return "offset";
		default: return String();
	}
}

int
ValueNode_GradientRotate::get_link_index_from_name(const String &name)const
{
	if(name=="gradient")
		return 0;
	if(name=="offset")
		return 1;
	throw Exception::BadLinkName(name);
}

String
ValueNode_GradientRotate::get_name()const
{
	return "gradient_rotate";
}

String
ValueNode_GradientRotate::get_local_name()const
{
	return _("Gradient Rotate");
}

bool
ValueNode_GradientRotate::check_type(ValueBase::Type type)
{
	return type==ValueBase::TYPE_GRADIENT;
}