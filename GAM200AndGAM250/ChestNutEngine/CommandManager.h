#pragma once
#include "UndoMove.h"
#include <stack>
#include "Input.h"
#include <iostream>

class CommandManager
{
public:
	CommandManager(InputSystem * in) : input{ in } {}

	~CommandManager()
	{
		while (undo_stack.size())
		{
			delete undo_stack.top();
			undo_stack.pop();
		}

		while (redo_stack.size())
		{
			delete redo_stack.top();
			redo_stack.pop();
		}
	}
	void Update()
	{
		if (input->InputCheckCurr(CVK_CTRL) && input->InputCheckTriggered(CVK_Z))
		{
			if (undo_stack.size())
			{
				Undo();
				std::cout << "Undo success" << std::endl;
			}

			else
				std::cout << "Undo stack is empty" << std::endl;
		}

		else if (input->InputCheckCurr(CVK_CTRL) && input->InputCheckTriggered(CVK_Y))
		{
			if (redo_stack.size())
			{
				Redo();
				std::cout << "Redo success" << std::endl;
			}

			else
				std::cout << "Redo stack is empty" << std::endl;
		}
	}

	void PushCommand(Command * cmd)
	{
		undo_stack.push(cmd);
	}
	
	void Undo()
	{
		undo_stack.top()->ExecuteUndo();
		redo_stack.push(undo_stack.top());
		undo_stack.pop();
	}


	void Redo()
	{
		
		redo_stack.top()->ExecuteRedo();
		delete redo_stack.top();
		redo_stack.pop();
	}

private:
	std::stack<Command *> undo_stack;
	std::stack<Command *> redo_stack;
	InputSystem * input;

};