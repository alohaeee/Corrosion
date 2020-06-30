#include "token_tree.hpp"

namespace corrosion
{
	void TokenStream::push(TreeAndJoint&& tree)
	{
		if (treeAndJoint.size() > 0)
		{
			auto prev_tree = treeAndJoint.back();

			if (prev_tree.second == IsJoint::Joint && prev_tree.first.isToken())
			{
				if (tree.first.isToken())
				{
					if (auto glued = tree.first.getToken().glue(prev_tree.first.getToken()); glued)
					{
						treeAndJoint.pop_back();
						treeAndJoint.emplace_back(std::move(glued.value()),IsJoint::Joint);
						return;
					}
				}
			}

		}
		treeAndJoint.push_back(tree);
	}

}