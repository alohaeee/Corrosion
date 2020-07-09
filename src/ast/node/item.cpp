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
	astLogPrint(fmt::format("ident: {}",ident.name().toString()),level+1);
	//level+=1;

	std::visit([this,level](auto&& arg)
	{
		using T = std::decay_t<decltype(arg)>;

		if constexpr(std::is_same_v<T, ItemKind::Const>)
		{
			astLogPrint("type: Const",level+1);
			astLogPrint("type:", level+1);
			if(arg.type)
			{
				arg.type->printer(level+2);
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
		else if constexpr(std::is_same_v<T, ItemKind::Fn>)
		{

			astLogPrint("type: Fn",level+1);
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
		else if constexpr(std::is_same_v<T, ItemKind::Static>)
		{
			astLogPrint("type: Static",level+1);
			if(arg.mut == Mutability::Mut)
			{
				astLogPrint("mutability: mut",level+1);
			}
			astLogPrint("type:", level+1);
			if(arg.type)
			{

				arg.type->printer(level+2);
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
	}, kind);
}

void corrosion::FnDecl::printer(std::size_t level)
{
	astLogPrint("fnDecl:", level);
	astLogPrint("params:", level+1);
	for(auto& p:param)
	{
		p.printer(level+2);
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
	astLogPrint("ret type:",level);
	if(type)
	{

		type->printer(level + 1);
	}
	else
	{
		astLogPrint("BUG: There must be type", level+1);
	}
	if(pat)
	{
		astLogPrint("pat:", level);
		pat->printer(level + 1);
	}
	else
	{
		astLogPrint("BUG: There must be pat", level+1);
	}
	if(isPlaceholder)
	{
		astLogPrint("_", level+1);
	}
}
