/******************************************************************************/
/*!
\file   BehaviourTree.h

\author Ong Tze Ang(100%)
\par    email: tzeang.ong\@digipen.edu
\par    DigiPen login: tzeang.ong

\brief
This file contains the interface of BehaviourTree

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#pragma once
#include <list>
#include "AIScript.h"
#include "RigidBodyComponent.h"
#include "macros.h"

// Abstract class
class Node
{
public:
	virtual bool run() const = 0;
private:
};

class CompositeNode : public Node
{
public:
	const std::list<Node*>& GetLeaf() const { return m_leaf; }
	void AddLeaf(Node* leaf) { m_leaf.emplace_back(leaf); }
private:
	std::list<Node*> m_leaf;
};

class Action : public Node
{
public:
	virtual bool run() const = 0;
private:
};

class ApproachPlayer : public Action
{
public:
	ApproachPlayer(RigidBodyComponent* p_rbComp, RigidBodyComponent* e_rbComp)
		:m_p_rbComp(p_rbComp), m_e_rbComp(e_rbComp)
	{ }
	virtual bool run() const override
	{
		//AIScript::MoveTowards_X(m_e_rbComp, m_p_rbComp, VALKYRIE_MOVESPEED);
		return true;
	}
private:
	RigidBodyComponent* m_p_rbComp, *m_e_rbComp;
};


class Selector : public CompositeNode
{
public:
	virtual bool run() const override
	{
		for (auto& e : GetLeaf())
		{
			if (e->run())
				return true;
		}
		return false;
	}
private:
};

class Sequence : public CompositeNode
{
public:
	virtual bool run() const override
	{
		for (auto& e : GetLeaf())
		{
			if (!e->run())
				return false;
		}
		return true;
	}
private:
};

