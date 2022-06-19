#include "utils.h"
#include <vector>
#include <algorithm>

std::vector<int> findCommon (std::vector<int> a, std::vector<int> b)
{
	std::vector<int> result;

	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	while(!a.empty() && !b.empty())
	{
		if (a.back() == b.back())
		{
			result.push_back(a.back());
			a.pop_back();
			b.pop_back();
		} else {
			if (a.back() > b.back())
			{
				a.pop_back();
			}else{
				b.pop_back();
			}
		}
	}

	return result;
}

int getMin (std::vector<int> a)
{
	int min = a[0];

	for (auto x : a)
	{
		if (x < min)
		{
			min = x;
		}
	}

	return min;
}