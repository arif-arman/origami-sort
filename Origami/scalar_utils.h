#pragma once

#include "commons.h"

namespace scalar_utils {
	template <typename Item>
	FORCEINLINE void swap(Item& v0, Item& v1) {
		const Item tmp = MIN(v0, v1);
		v1 = MAX(v0, v1);
		v0 = tmp;
	}
}