#include "stmt.hpp"
#include "item.hpp"

void corrosion::Block::printer(std::size_t level)
{
	auto label = nodeFormatter("Block", id, span);
	astLogPrint(label, level);
	level+=1;
	for(auto& stmt:stmts)
	{
		stmt.printer(level);
	}
}

void corrosion::Item::printer(std::size_t level)
{
	auto label = nodeFormatter("Item", id, span);
	astLogPrint(label, level);
	//level+=1;

	std::visit([this,level](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;

		if constexpr(std::is_same_v<T, ItemKind::Const>)
		{
			astLogPrint("type: Const",level+1);
			if(arg.type)
			{
				arg.type->printer(level+1);
			}
			else
			{
				astLogPrint("BUG: There must be type", level+2);
			}
			if(arg.expr)
			{
				astLogPrint("expr:", level+1);
				arg.expr->printer(level+2);
			}
		}
		if constexpr(std::is_same_v<T, ItemKind::Fn>)
		{
			astLogPrint("sig:", level+1);
			arg.sig.decl->printer(level+2);
			if(arg.block)
			{
				astLogPrint("block:", level+1);
				arg.block->printer(level+2);
			}
			else
			{
				astLogPrint("BUG: There must be block", level+2);
			}
		}
	}, kind);
}

void corrosion::FnDecl::printer(std::size_t level)
{
	astLogPrint("fnDecl:", level);
	for(auto& Param:param)
	{
		Param.printer(level+1);
	}
	if(returnType)
	{
		astLogPrint("returnType:", level+1);
		returnType->printer(level+2);
	}
}

void corrosion::Param::printer(std::size_t level)
{
	auto label = nodeFormatter("Param", id, span);
	astLogPrint(label, level);
	level+=1;

	if(type)
	{
		type->printer(level + 1);
	}
	else
	{
		astLogPrint("BUG: There must be type", level+2);
	}
	if(pat)
	{
		astLogPrint("pat:", level+1);
		pat->printer(level + 2);
	}
	else
	{
		astLogPrint("BUG: There must be pat", level+2);
	}
	if(isPlaceholder)
	{
		astLogPrint("_", level+1);
	}
}
