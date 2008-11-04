/* === S Y N F I G ========================================================= */
/*!	\file valuenode_dotproduct.cpp
**	\brief Implementation of the "DotProduct" valuenode conversion.
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2008 Chris Moore
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

#include "valuenode_dotproduct.h"
#include "valuenode_const.h"
#include "general.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

ValueNode_DotProduct::ValueNode_DotProduct(const ValueBase &value):
	LinkableValueNode(value.get_type())
{
	switch(value.get_type())
	{
	case ValueBase::TYPE_REAL:
		set_link("lhs",ValueNode_Const::create(Vector(value.get(Real()),0)));
		set_link("rhs",ValueNode_Const::create(Vector(1,0)));
		break;
	case ValueBase::TYPE_ANGLE:
		set_link("lhs",ValueNode_Const::create(Vector(Angle::cos(value.get(Angle())).get(), Angle::sin(value.get(Angle())).get())));
		set_link("rhs",ValueNode_Const::create(Vector(1,0)));
		break;
	default:
		throw Exception::BadType(ValueBase::type_local_name(value.get_type()));
	}

	DCAST_HACK_ENABLE();
}

LinkableValueNode*
ValueNode_DotProduct::create_new()const
{
	return new ValueNode_DotProduct(get_type());
}

ValueNode_DotProduct*
ValueNode_DotProduct::create(const ValueBase &x)
{
	return new ValueNode_DotProduct(x);
}

ValueNode_DotProduct::~ValueNode_DotProduct()
{
	unlink_all();
}

ValueBase
ValueNode_DotProduct::operator()(Time t)const
{
	Vector lhs((*lhs_)(t).get(Vector()));
	Vector rhs((*rhs_)(t).get(Vector()));

	switch (get_type())
	{
	case ValueBase::TYPE_ANGLE:
		return Angle::cos(lhs * rhs / lhs.mag() / rhs.mag()).mod();
	case ValueBase::TYPE_REAL:
		return lhs * rhs;
	default:
		break;
	}

	assert(0);
	return ValueBase();
}

String
ValueNode_DotProduct::get_name()const
{
	return "dotproduct";
}

String
ValueNode_DotProduct::get_local_name()const
{
	return _("Dot Product");
}

bool
ValueNode_DotProduct::set_link_vfunc(int i,ValueNode::Handle value)
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: CHECK_TYPE_AND_SET_VALUE(lhs_, ValueBase::TYPE_VECTOR);
	case 1: CHECK_TYPE_AND_SET_VALUE(rhs_, ValueBase::TYPE_VECTOR);
	}
	return false;
}

ValueNode::LooseHandle
ValueNode_DotProduct::get_link_vfunc(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: return lhs_;
	case 1: return rhs_;
	}

	return 0;
}

int
ValueNode_DotProduct::link_count()const
{
	return 2;
}

String
ValueNode_DotProduct::link_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return "lhs";
		case 1: return "rhs";
	}
	return String();
}

String
ValueNode_DotProduct::link_local_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return _("LHS");
		case 1: return _("RHS");
	}
	return String();
}

int
ValueNode_DotProduct::get_link_index_from_name(const String &name)const
{
	if (name=="lhs") return 0;
	if (name=="rhs") return 1;

	throw Exception::BadLinkName(name);
}

bool
ValueNode_DotProduct::check_type(ValueBase::Type type)
{
	return
		type==ValueBase::TYPE_ANGLE ||
		type==ValueBase::TYPE_REAL;
}