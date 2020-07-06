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

