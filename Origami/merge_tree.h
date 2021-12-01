/*--------------------------------------------------------------------------------------------
 - Origami: A High-Performance Mergesort Framework											 -
 - Copyright(C) 2021 Arif Arman, Dmitri Loguinov											 -
 - Produced via research carried out by the Texas A&M Internet Research Lab                  -
 -                                                                                           -
 - This program is free software : you can redistribute it and/or modify                     -
 - it under the terms of the GNU General Public License as published by                      -
 - the Free Software Foundation, either version 3 of the License, or                         -
 - (at your option) any later version.                                                       -
 -                                                                                           -
 - This program is distributed in the hope that it will be useful,                           -
 - but WITHOUT ANY WARRANTY; without even the implied warranty of                            -
 - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                               -
 - GNU General Public License for more details.                                              -
 -                                                                                           -
 - You should have received a copy of the GNU General Public License                         -
 - along with this program. If not, see < http://www.gnu.org/licenses/>.                     -
 --------------------------------------------------------------------------------------------*/

#pragma once

#include "commons.h"
#include "merge_utils.h"

namespace origami_merge_tree {

#if MTREE_NREG == 1
	template <typename Reg, typename Item>
	FORCEINLINE void merge_leaf_to_internal(Item** _loadFrom, Item** _opposite, Reg& a0, Reg& a1, Item* outbuf, Item* endA, Item* endB, Item** _endoutput, ui& exhaust) {
		// exhaust 0 -> both children have Items; 1 -> loadFrom empty; 2 -> opposite empty i.e. all empty
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* endoutput = *_endoutput;
		if (outbuf != endoutput && exhaust < 2) {
			Item* loadFrom = *_loadFrom;
			Item* opposite = *_opposite;
			// run if both children have Items
			if (exhaust == 0) {
				while (outbuf != endoutput && loadFrom != endA && loadFrom != endB) {
					//printf("%llX, %llX\n", outbuf, endoutput);
					bool first = *loadFrom < *opposite;
					Item* tmp0 = first ? loadFrom : opposite;
					opposite = first ? opposite : loadFrom;
					loadFrom = tmp0;

					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf); 
					outbuf += ITEMS_PER_REG;
					origami_utils::load<Reg>(a0, (Reg*)loadFrom);
					_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
					loadFrom += ITEMS_PER_REG;
				}
			}
			// if one of the streams run out
			if (outbuf != endoutput) {
				if (!exhaust) {
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
					exhaust = 1;
					//printf("Leaf exhausted\n");
				}
				Item* endOpposite = (loadFrom == endA) ? endB : endA;
				while (outbuf != endoutput && opposite != endOpposite) {
					origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
				}
				if (outbuf != endoutput && exhaust != 2) {
					origami_utils::store<Reg, false>(a1, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
					exhaust = 2;
					endoutput = outbuf;		// we ran out of items so update end boundary
					//printf("Both leaf exhausted\n");
				}
			}
			*_loadFrom = loadFrom;
			*_opposite = opposite;
			*_endoutput = endoutput;
		}
	}

	// external
	template <typename Reg, typename Item>
	FORCEINLINE void merge_leaf_to_internal(Item** _loadFrom, Item** _opposite, Reg& a0, Reg& a1, Item* outbuf, Item* endA, Item* endB, Item** _endoutput, ui& exhaust, ui base_idx, origami_utils::IOHelper* IO) {
		//printf("Merging leaf to internal @ %lu ... \n", base_idx);
		// exhaust 0 -> both children have Items; 1 -> loadFrom empty; 2 -> opposite empty i.e. all empty
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* endoutput = *_endoutput;
		if (outbuf != endoutput && exhaust < 2) {
			Item* loadFrom = *_loadFrom;
			Item* opposite = *_opposite;
			// run if both children have Items
			if (exhaust == 0) {
				while (true) {
					while (outbuf != endoutput && loadFrom != endA && loadFrom != endB) {
						//printf("%llX, %llX, %llX, %llX, %llX\n", outbuf, endoutput, loadFrom, endA, endB);
						bool first = *loadFrom < *opposite;
						Item* tmp0 = first ? loadFrom : opposite;
						opposite = first ? opposite : loadFrom;
						loadFrom = tmp0;

						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf);		//origami_utils::print<Reg, Item>(a0);
						outbuf += ITEMS_PER_REG;
						origami_utils::load<Reg>(a0, (Reg*)loadFrom);
						_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
						loadFrom += ITEMS_PER_REG;
					}
					// buffer full
					if (outbuf == endoutput) break;
					// else: some input buffer ran out
					ui empty_idx = (loadFrom == endA) ? base_idx : (base_idx + 1);
					//printf("Buffer %lu empty\n", empty_idx);
					if (IO->bytes_left[empty_idx] > 0) {
						IO->fill_buffer(empty_idx);
						loadFrom = (Item*)IO->X[empty_idx];
					}
					else {
						//printf("** Node %lu exhausted\n", empty_idx);
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
						exhaust = 1;
						break;
					}
				}
			}
			
			if (outbuf != endoutput) {
				Item* endOpposite = (loadFrom == endA) ? endB : endA;
				ui opposite_idx = (loadFrom == endA) ? (base_idx + 1) : base_idx;
				while (true) {
					while (outbuf != endoutput && opposite != endOpposite) {
						origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
					}
					if (outbuf == endoutput) break;
					// refill opposite buffer
					//printf("Buffer %lu empty\n", opposite_idx);
					if (IO->bytes_left[opposite_idx] > 0) {
						IO->fill_buffer(opposite_idx);
						opposite = (Item*)IO->X[opposite_idx];
					}
					else {
						//printf("** Node %lu exhausted\n", opposite_idx);
						origami_utils::store<Reg, false>(a1, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
						exhaust = 2;
						endoutput = outbuf;
						break;
					}
				}
			}
			*_loadFrom = loadFrom;
			*_opposite = opposite;
			*_endoutput = endoutput;
		}
	}

	template <typename Reg, typename Item>
	FORCEINLINE Item* merge_internal_to_internal(Item** _loadFrom, Item** _opposite, Reg& a0, Reg& a1, Item* outbuf, Item* endA, Item* endB, Item** _endoutput, ui exhaust0, ui exhaust1, ui& exhaust) {
		//printf("Merging internal to internal ... \n");
		// exhaust 0 -> both children have keys; 1 -> loadFrom empty; 2 -> opposite empty i.e. all empty
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		if (exhaust < 2) {
			Item* endoutput = *_endoutput;
			Item* loadFrom = *_loadFrom;
			Item* opposite = *_opposite;

			// run if both children have keys
			while (outbuf != endoutput && loadFrom != endA && loadFrom != endB) {
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				//		PAVX(a0); PAVX(a1);
				origami_utils::rswap<Reg, Item>(a0, a1);				//origami_utils::print<Reg, Item>(a0);
				origami_utils::store<Reg, false>(a0, (Reg*)outbuf); 
				outbuf += ITEMS_PER_REG;	
				//PAVX(a0);// PAVX(a1);
				origami_utils::load<Reg>(a0, (Reg*)loadFrom);
				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				loadFrom += ITEMS_PER_REG;
			}

			// loadFrom exhausted
			if (outbuf != endoutput) {
				// this was the last batch from A
				if (exhaust0 == 2 && loadFrom == endA) {
					if (!exhaust) {
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
						exhaust = 1;
					}
					Item* endOpposite = endB;
					while (outbuf != endoutput && opposite != endOpposite) {
						origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
					}
					if (outbuf != endoutput) {
						if (exhaust1 == 2) {
							origami_utils::store<Reg, false>(a1, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
							exhaust = 2;
							endoutput = outbuf;
						}
						Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
					}

				}
				// or last batch from B
				else if (exhaust1 == 2 && loadFrom == endB) {
					if (!exhaust) {
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
						exhaust = 1;
					}
					Item* endOpposite = endA;
					while (outbuf != endoutput && opposite != endOpposite) {
						origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, false>(a0, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
					}
					if (outbuf != endoutput) {
						if (exhaust0 == 2) {
							origami_utils::store<Reg, false>(a1, (Reg*)outbuf); outbuf += ITEMS_PER_REG;
							exhaust = 2;
							endoutput = outbuf;
						}
						Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
					}
				}
			}
			/*else {
				endoutput = outbuf;
			}*/

			*_loadFrom = loadFrom;
			*_opposite = opposite;
			*_endoutput = endoutput;
		}
		else *_endoutput = outbuf;
		//printf("Exiting merge internal to internal ... \n");
		return outbuf;
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_root_aligned(Item** _loadFrom, Item** _opposite, Reg& a1, Item** _output, Item* endA, Item* endB, ui exhaust0, ui exhaust1) {
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;
		Item* output = *_output;
		register Reg a0;

		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//printf("Exhaust0: %lu, Exhaust1: %lu\n", exhaust0, exhaust1); 

		while (loadFrom != endA && loadFrom != endB) {
			origami_utils::load<Reg>(a0, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, true>(a0, (Reg*)output);
			output += ITEMS_PER_REG;
			//PAVX(a0);
		}
		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		// handle tail
		if ((exhaust0 == 2 && loadFrom == endA)) {
			//printf("Handling tail\n");
			Item* endOpposite = endB;
			while (opposite != endOpposite) {
				origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
			}
			if (exhaust1 == 2) {
				origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
			}
			Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;	// std::swap is expensive
		}
		else if ((exhaust1 == 2 && loadFrom == endB)) {
			//printf("Handling tail\n");
			Item* endOpposite = endA;
			while (opposite != endOpposite) {
				origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
			}
			if (exhaust0 == 2) {
				origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
			}
			Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
		}
		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*_output = output;
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_root_unaligned(Item** _loadFrom, Item** _opposite, Reg& a1, Item** _output, Item* endA, Item* endB, ui exhaust0, ui exhaust1, Item* outputEnd) {
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;
		Item* output = *_output;
		register Reg a0;

		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//printf("Exhaust0: %lu, Exhaust1: %lu\n", exhaust0, exhaust1); 

		while (loadFrom != endA && loadFrom != endB && output != outputEnd) { // 
			origami_utils::load<Reg>(a0, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			origami_utils::rswap<Reg, Item>(a0, a1);
			origami_utils::store<Reg, true>(a0, (Reg*)output);


			/*Item* newoutput = output + ITEMS_PER_REG;
			ui prior_unalign_items_new = prior_unalign_items_loc - ITEMS_PER_REG;
			output = prior_unalign_items_loc ? output : newoutput;
			prior_unalign_items_loc = prior_unalign_items_loc ? prior_unalign_items_new : prior_unalign_items_loc; */

			output += ITEMS_PER_REG;
		}

		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		// handle tail
		if (output != outputEnd) {
			if (exhaust0 == 2 && loadFrom == endA) {
				//printf("Handling tail\n");
				Item* endOpposite = endB;
				while (opposite != endOpposite && output != outputEnd) {
					origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
				}
				if (exhaust1 == 2 && output != outputEnd) {
					origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
				}
				Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;	// std::swap is expensive
			}
			else if (exhaust1 == 2 && loadFrom == endB) {
				//printf("Handling tail\n");
				Item* endOpposite = endA;
				while (opposite != endOpposite && output != outputEnd) {
					origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
					origami_utils::rswap<Reg, Item>(a0, a1);
					origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
				}
				if (exhaust0 == 2 && output != outputEnd) {
					origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
				}
				Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
			}
		}
		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*_output = output;
	}

	// external
	template <typename Reg, typename Item>
	FORCEINLINE void merge_root_unaligned(Item** _loadFrom, Item** _opposite, Reg& a1, Item** _output, Item* endA, Item* endB, ui exhaust0, ui exhaust1, Item* outputEnd, origami_utils::IOHelper* IO) {
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;
		Item* output = *_output;
		register Reg a0;

		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//printf("Exhaust0: %lu, Exhaust1: %lu\n", exhaust0, exhaust1); 

		while (true) {
			//printf("Merging at root, left: %llu ... \n", outputEnd - output);
			while (loadFrom != endA && loadFrom != endB && output != outputEnd) { // 
				origami_utils::load<Reg>(a0, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				origami_utils::rswap<Reg, Item>(a0, a1);
				origami_utils::store<Reg, true>(a0, (Reg*)output);				//origami_utils::print<Reg, Item>(a0);


				/*Item* newoutput = output + ITEMS_PER_REG;
				ui prior_unalign_items_new = prior_unalign_items_loc - ITEMS_PER_REG;
				output = prior_unalign_items_loc ? output : newoutput;
				prior_unalign_items_loc = prior_unalign_items_loc ? prior_unalign_items_new : prior_unalign_items_loc; */

				output += ITEMS_PER_REG;
			}
			if (output != outputEnd) break;
			// output buffer full
			if (output == outputEnd) {
				//printf("Output buffer full ... writing to file\n");
				IO->dump_buffer(0, (char*)output);
				output = (Item*)IO->out;
				continue;
			}
		}
				

		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		// handle tail
		if (output != outputEnd) {
			if (exhaust0 == 2 && loadFrom == endA) {
				// printf("Handling tail -- loadFrom == endA\n");
				Item* endOpposite = endB;
				while (true) {
					while (opposite != endOpposite && output != outputEnd) {
						origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
					}
					if (output == outputEnd) {
						//printf("Output buffer full ... writing to file\n");
						IO->dump_buffer(0, (char*)output);
						output = (Item*)IO->out;
						continue;
					}
					if (exhaust1 == 2) {
						origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
						IO->dump_buffer(0, (char*)output);
						output = outputEnd;
					}
					break;
				}
				Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;	// std::swap is expensive
			}
			else if (exhaust1 == 2 && loadFrom == endB) {
				// printf("Handling tail -- loadFrom == endB\n");
				Item* endOpposite = endA;
				while (true) {
					while (opposite != endOpposite && output != outputEnd) {
						origami_utils::load<Reg>(a0, (Reg*)opposite); opposite += ITEMS_PER_REG;
						origami_utils::rswap<Reg, Item>(a0, a1);
						origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
					}
					if (output == outputEnd) {
						//printf("Output buffer full ... writing to file\n");
						IO->dump_buffer(0, (char*)output);
						output = (Item*)IO->out;
						continue;
					}
					if (exhaust0 == 2) {
						origami_utils::store<Reg, true>(a1, (Reg*)output); output += ITEMS_PER_REG;
						IO->dump_buffer(0, (char*)output);
						output = outputEnd;
					}
					break;
				}
				Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
			}
		}
		//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
		//origami_utils::store<Reg, true>(a0, (Reg*)output); output += ITEMS_PER_REG;
		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*_output = output;
	}

	template <typename Reg, typename Item>
	FORCEINLINE void merge_root_unaligned_skip_prior(Item** _loadFrom, Item** _opposite, Reg& a1, Item* endA, Item* endB, ui exhaust0, ui exhaust1, ui* prior_unalign_items) {
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;
		ui prior_unalign_items_loc = *prior_unalign_items;
		register Reg a0;

		while (loadFrom != endA && loadFrom != endB && prior_unalign_items_loc > 0) { // 
			origami_utils::load<Reg>(a0, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			origami_utils::rswap<Reg, Item>(a0, a1);

			prior_unalign_items_loc -= ITEMS_PER_REG;
		}

		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*prior_unalign_items = prior_unalign_items_loc;
	}

	template <typename Reg, typename Item, bool external = false>
	class Merge4Way {
	public:
		Item* l1Buf0, * l1Buf1, * l1endBuf0, * l1endBuf1;
		Item* output, * outputEnd;
		Reg a0, a1, a2, a3, a4, a5;
		Item* leafEnd0, * leafEnd1, * leafEnd2, * leafEnd3;
		Item* loadFrom0, * opposite0, * loadFrom1, * opposite1, * loadFrom, * opposite;
		ui exhaust0, exhaust1, exhaust2, exhaust3, exhaust4, exhaust5, exhaust;
		Item* l1endBuf0_back, * l1endBuf1_back, * outputEnd_back;

		void print_node() {
			printf("[%llX %llX]\n", output, outputEnd);
			printf("[%llX %llX] [%llX %llX]\n", l1Buf0, l1endBuf0, l1Buf1, l1endBuf1);
			printf("[%llX %llX] [%llX %llX] [%llX %llX] [%llX %llX]\n", loadFrom0, leafEnd0, opposite0, leafEnd1, loadFrom1, leafEnd2, opposite1, leafEnd3);
		}

		void merge_leaf_to_root_init(ui base_idx = 0, origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;

			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;

			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			origami_utils::load<Reg>(a0, (Reg*)loadFrom0); loadFrom0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a1, (Reg*)opposite0); opposite0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a2, (Reg*)loadFrom1); loadFrom1 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a3, (Reg*)opposite1); opposite1 += ITEMS_PER_REG;

			if constexpr (external) {
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, base_idx, IO);
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, base_idx + 2, IO);
			}
			else {
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
			}


			origami_utils::load<Reg>(a5, (Reg*)opposite); opposite += ITEMS_PER_REG;

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a5 = a5;

			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;
		}

		void merge_leaf_to_internal_init(ui base_idx = 0, origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;

			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;

			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			origami_utils::load<Reg>(a0, (Reg*)loadFrom0); loadFrom0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a1, (Reg*)opposite0); opposite0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a2, (Reg*)loadFrom1); loadFrom1 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a3, (Reg*)opposite1); opposite1 += ITEMS_PER_REG;

			if constexpr (external) {
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, base_idx, IO);
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, base_idx + 2, IO);
			}
			else {
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
			}

			

			origami_utils::load<Reg>(a4, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			origami_utils::load<Reg>(a5, (Reg*)opposite); opposite += ITEMS_PER_REG;

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;
		}

		void merge_internal_to_internal_init() {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;

			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;

			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust0 = this->exhaust0;
			ui exhaust1 = this->exhaust1;
			ui exhaust2 = this->exhaust2;
			ui exhaust3 = this->exhaust3;
			ui exhaust4 = this->exhaust4;
			ui exhaust5 = this->exhaust5;

			origami_utils::load<Reg>(a0, (Reg*)loadFrom0); loadFrom0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a1, (Reg*)opposite0); opposite0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a2, (Reg*)loadFrom1); loadFrom1 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a3, (Reg*)opposite1); opposite1 += ITEMS_PER_REG;

			origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
			origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);


			origami_utils::load<Reg>(a4, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			origami_utils::load<Reg>(a5, (Reg*)opposite); opposite += ITEMS_PER_REG;

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;

			this->exhaust4 = exhaust4;
			this->exhaust5 = exhaust5;
		}

		void merge_internal_to_root_init() {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;

			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;

			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust4 = this->exhaust4;
			ui exhaust5 = this->exhaust5;

			origami_utils::load<Reg>(a0, (Reg*)loadFrom0); loadFrom0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a1, (Reg*)opposite0); opposite0 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a2, (Reg*)loadFrom1); loadFrom1 += ITEMS_PER_REG;
			origami_utils::load<Reg>(a3, (Reg*)opposite1); opposite1 += ITEMS_PER_REG;

			origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
			origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);

			//origami_utils::load<Reg>(a4, (Reg*)loadFrom); loadFrom += ITEMS_PER_REG;
			origami_utils::load<Reg>(a5, (Reg*)opposite); opposite += ITEMS_PER_REG;

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			//this->a4 = a4;
			this->a5 = a5;

			this->exhaust4 = exhaust4;
			this->exhaust5 = exhaust5;
		}

		void merge_leaf_to_root(origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;
			Item* output = this->output;
			Item* outputEnd = this->outputEnd;
			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;
			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			

			// merge
			Item* o = output;
			while (output < outputEnd) {
				merge_root_aligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust0, exhaust1);
				//printf("Merged: %llu\n", output - o);

				// refill empty child
				if (exhaust0 < 2 && loadFrom == l1endBuf0) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, 0, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
					loadFrom = l1Buf0;
				}
				else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, 2, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
					loadFrom = l1Buf1;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;
			}
			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;

			this->output = output;
			this->outputEnd = outputEnd;
			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a5 = a5;
		}

		void merge_leaf_to_root_unaligned(ui* prior_unalign_items, origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;
			Item* output = this->output;
			Item* outputEnd = this->outputEnd;
			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a5 = this->a5;
			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			// merge

			// skip prior unaligned items
			while (1) {
				merge_root_unaligned_skip_prior(&loadFrom, &opposite, a5, l1endBuf0, l1endBuf1, exhaust0, exhaust1, prior_unalign_items);
				if (*prior_unalign_items == 0) break;
				// refill empty child
				if (exhaust0 < 2 && loadFrom == l1endBuf0) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, 0, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
					loadFrom = l1Buf0;
				}
				else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, 2, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
					loadFrom = l1Buf1;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;
			}


			Item* o = output;
			while (output < outputEnd) {
				if constexpr (external) merge_root_unaligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust0, exhaust1, outputEnd, IO);
				else merge_root_unaligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust0, exhaust1, outputEnd);
				//printf("Merged: %llu\n", output - o);

				// refill empty child
				if (exhaust0 < 2 && loadFrom == l1endBuf0) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, 0, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
					loadFrom = l1Buf0;
				}
				else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, 2, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
					loadFrom = l1Buf1;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;
			}
			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;

			this->output = output;
			this->outputEnd = outputEnd;
			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a5 = a5;
		}

		void merge_leaf_to_internal(ui base_idx = 0, origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;

			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;

			Item* output = this->output;
			Item* outputEnd = this->outputEnd;

			//printf("Filling leaf to internal: [%llX %llX]\n", output, outputEnd);

			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a4 = this->a4;
			Reg a5 = this->a5;

			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust0 = this->exhaust0;
			ui exhaust1 = this->exhaust1;
			ui exhaust = this->exhaust;

			// merge
			ui64 cnt = 0;
			while (output < outputEnd) {
				//do {
					// refill empty child
				if (exhaust0 < 2 && loadFrom == l1endBuf0) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, base_idx, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
					loadFrom = l1Buf0;
				}
				else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
					if constexpr (external) origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1, base_idx + 2, IO);
					else origami_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
					loadFrom = l1Buf1;
				}
				bool first = *(loadFrom) < *(opposite);
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;
				//Item* o = output;
				output = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom, &opposite, a4, a5, output, l1endBuf0, l1endBuf1, &outputEnd, exhaust0, exhaust1, exhaust);
				//PRINT_ARR64(o, output - o); cnt += (output - o); printf("Tot: %llu\n", cnt);
			}
			//} while (output < outputEnd);
			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->exhaust0 = exhaust0;
			this->exhaust1 = exhaust1;
			this->exhaust = exhaust;

			this->outputEnd = outputEnd;
			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;
		}

		ui merge_internal_to_internal() {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;
			Item* output = this->output;
			Item* outputEnd = this->outputEnd;
			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a4 = this->a4;
			Reg a5 = this->a5;
			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust0 = this->exhaust0;
			ui exhaust1 = this->exhaust1;
			ui exhaust2 = this->exhaust2;
			ui exhaust3 = this->exhaust3;
			ui exhaust4 = this->exhaust4;
			ui exhaust5 = this->exhaust5;
			ui exhaust = this->exhaust;

			ui flag = 0;
			outputEnd = outputEnd_back;

			do {
				// refill empty child
				if (exhaust4 < 2 && loadFrom == l1endBuf0) {
					l1endBuf0 = l1endBuf0_back;
					l1endBuf0 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
					loadFrom = l1Buf0;
				}
				else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
					l1endBuf1 = l1endBuf1_back;
					l1endBuf1 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
					loadFrom = l1Buf1;
				}
				bool first = *(loadFrom) < *(opposite);
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				output = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom, &opposite, a4, a5, output, l1endBuf0, l1endBuf1, &outputEnd, exhaust4, exhaust5, exhaust);
				//PRINT_ARR(o, output - o);

				// check if any leaf node is empty
				ui empty0 = loadFrom0 == leafEnd0;
				ui empty1 = loadFrom0 == leafEnd1;
				ui empty2 = loadFrom1 == leafEnd2;
				ui empty3 = loadFrom1 == leafEnd3;
				flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
				//printf("Flag: %llx\n", flag);

			} while (flag == 0 && output < outputEnd);
			outputEnd = output;

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			//this->output = output;
			this->outputEnd = outputEnd;

			this->exhaust0 = exhaust0;
			this->exhaust1 = exhaust1;
			this->exhaust2 = exhaust2;
			this->exhaust3 = exhaust3;
			this->exhaust4 = exhaust4;
			this->exhaust5 = exhaust5;
			this->exhaust = exhaust;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;

			return flag;
		}

		ui merge_internal_to_root(origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;
			Item* output = this->output;
			Item* outputEnd = this->outputEnd;
			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a4 = this->a4;
			Reg a5 = this->a5;
			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust0 = this->exhaust0;
			ui exhaust1 = this->exhaust1;
			ui exhaust2 = this->exhaust2;
			ui exhaust3 = this->exhaust3;
			ui exhaust4 = this->exhaust4;
			ui exhaust5 = this->exhaust5;
			ui exhaust = this->exhaust;
			// merge
			ui flag = 0;
			do {
				// refill empty child
				if (exhaust4 < 2 && loadFrom == l1endBuf0) {
					l1endBuf0 = l1endBuf0_back;
					l1endBuf0 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
					loadFrom = l1Buf0;
				}
				else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
					l1endBuf1 = l1endBuf1_back;
					l1endBuf1 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
					loadFrom = l1Buf1;
				}
				bool first = *(loadFrom) < *(opposite);
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				// from L1 to root

				Item* o = output;
				if constexpr (external) merge_root_unaligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5, outputEnd, IO);
				else merge_root_aligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5);

				//PRINT_ARR(o, output - o);

				// check if any leaf node is empty
				ui empty0 = (loadFrom0 == leafEnd0);// && (exhaust0 < 2);
				ui empty1 = (loadFrom0 == leafEnd1);// && (exhaust1 < 2);
				ui empty2 = (loadFrom1 == leafEnd2);// && (exhaust2 < 2);
				ui empty3 = (loadFrom1 == leafEnd3);// && (exhaust3 < 2);
				flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
				//printf("Flag: %llx\n", flag);

			} while (flag == 0 && output < outputEnd);

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->output = output;
			this->outputEnd = outputEnd;

			this->exhaust0 = exhaust0;
			this->exhaust1 = exhaust1;
			this->exhaust2 = exhaust2;
			this->exhaust3 = exhaust3;
			this->exhaust4 = exhaust4;
			this->exhaust5 = exhaust5;
			this->exhaust = exhaust;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;

			return flag;
		}

		ui merge_internal_to_root_unaligned(ui* prior_unalign_items, origami_utils::IOHelper* IO = nullptr) {
			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			Item* loadFrom0 = this->loadFrom0;
			Item* loadFrom1 = this->loadFrom1;
			Item* loadFrom = this->loadFrom;
			Item* opposite0 = this->opposite0;
			Item* opposite1 = this->opposite1;
			Item* opposite = this->opposite;
			Item* l1Buf0 = this->l1Buf0;
			Item* l1Buf1 = this->l1Buf1;
			Item* l1endBuf0 = this->l1endBuf0;
			Item* l1endBuf1 = this->l1endBuf1;
			Item* output = this->output;
			Item* outputEnd = this->outputEnd;
			Reg a0 = this->a0;
			Reg a1 = this->a1;
			Reg a2 = this->a2;
			Reg a3 = this->a3;
			Reg a4 = this->a4;
			Reg a5 = this->a5;
			Item* leafEnd0 = this->leafEnd0;
			Item* leafEnd1 = this->leafEnd1;
			Item* leafEnd2 = this->leafEnd2;
			Item* leafEnd3 = this->leafEnd3;

			ui exhaust0 = this->exhaust0;
			ui exhaust1 = this->exhaust1;
			ui exhaust2 = this->exhaust2;
			ui exhaust3 = this->exhaust3;
			ui exhaust4 = this->exhaust4;
			ui exhaust5 = this->exhaust5;
			ui exhaust = this->exhaust;

			// merge

			ui flag = 0;
			// skip over prior partition keys
			do {
				// refill empty child
				if (exhaust4 < 2 && loadFrom == l1endBuf0) {
					l1endBuf0 = l1endBuf0_back;
					l1endBuf0 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
					loadFrom = l1Buf0;
				}
				else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
					l1endBuf1 = l1endBuf1_back;
					l1endBuf1 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
					loadFrom = l1Buf1;
				}
				bool first = *(loadFrom) < *(opposite);
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				Item* o = output;
				merge_root_unaligned_skip_prior(&loadFrom, &opposite, a5, l1endBuf0, l1endBuf1, exhaust4, exhaust5, prior_unalign_items);

				// check if any leaf node is empty
				ui empty0 = (loadFrom0 == leafEnd0);
				ui empty1 = (loadFrom0 == leafEnd1);
				ui empty2 = (loadFrom1 == leafEnd2);
				ui empty3 = (loadFrom1 == leafEnd3);
				flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
				//printf("Flag: %llx\n", flag);

			} while (flag == 0 && *prior_unalign_items > 0);

			// previous parition keys skipped
			do {
				// refill empty child
				if (exhaust4 < 2 && loadFrom == l1endBuf0) {
					l1endBuf0 = l1endBuf0_back;
					l1endBuf0 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
					loadFrom = l1Buf0;
				}
				else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
					l1endBuf1 = l1endBuf1_back;
					l1endBuf1 = origami_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a2, a3, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
					loadFrom = l1Buf1;
				}
				bool first = *(loadFrom) < *(opposite);
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;

				// from L1 to root

				Item* o = output;
				if constexpr (external) merge_root_unaligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5, outputEnd, IO);
				else merge_root_unaligned(&loadFrom, &opposite, a5, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5, outputEnd);

				// check if any leaf node is empty
				ui empty0 = (loadFrom0 == leafEnd0);// && (exhaust0 < 2);
				ui empty1 = (loadFrom0 == leafEnd1);// && (exhaust1 < 2);
				ui empty2 = (loadFrom1 == leafEnd2);// && (exhaust2 < 2);
				ui empty3 = (loadFrom1 == leafEnd3);// && (exhaust3 < 2);
				flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
				//printf("Flag: %llx\n", flag);

			} while (flag == 0 && output < outputEnd);

			this->loadFrom0 = loadFrom0;
			this->loadFrom1 = loadFrom1;
			this->loadFrom = loadFrom;
			this->opposite0 = opposite0;
			this->opposite1 = opposite1;
			this->opposite = opposite;

			this->l1Buf0 = l1Buf0;
			this->l1Buf1 = l1Buf1;
			this->l1endBuf0 = l1endBuf0;
			this->l1endBuf1 = l1endBuf1;

			this->output = output;
			this->outputEnd = outputEnd;

			this->exhaust0 = exhaust0;
			this->exhaust1 = exhaust1;
			this->exhaust2 = exhaust2;
			this->exhaust3 = exhaust3;
			this->exhaust4 = exhaust4;
			this->exhaust5 = exhaust5;
			this->exhaust = exhaust;

			this->a0 = a0;
			this->a1 = a1;
			this->a2 = a2;
			this->a3 = a3;
			this->a4 = a4;
			this->a5 = a5;

			return flag;
		}

		void initialize(Item* _leafBuf0, Item* _leafBuf1, Item* _leafBuf2, Item* _leafBuf3,
			Item* _leafEnd0, Item* _leafEnd1, Item* _leafEnd2, Item* _leafEnd3,
			Item* _l1Buf0, Item* _l1Buf1, Item* _l1endBuf0, Item* _l1endBuf1,
			Item* _output, Item* _outputEnd) {

			loadFrom0 = _leafBuf0;	opposite0 = _leafBuf1;  loadFrom1 = _leafBuf2;  opposite1 = _leafBuf3;
			leafEnd0 = _leafEnd0;	leafEnd1 = _leafEnd1; 	leafEnd2 = _leafEnd2; 	leafEnd3 = _leafEnd3;

			l1Buf0 = _l1Buf0;	l1Buf1 = _l1Buf1; 	l1endBuf0 = _l1endBuf0; l1endBuf1 = _l1endBuf1;
			l1endBuf0_back = _l1endBuf0; l1endBuf1_back = _l1endBuf1;

			loadFrom = l1Buf0; opposite = l1Buf1;

			output = _output; outputEnd = _outputEnd; outputEnd_back = _outputEnd;

			exhaust0 = exhaust1 = exhaust2 = exhaust3 = exhaust4 = exhaust5 = exhaust = 0;
		}
	};

#elif MTREE_NREG == 2

template <typename Reg, typename Item>
FORCEINLINE void merge_leaf_to_internal(Item** _loadFrom, Item** _opposite, Reg& a0, Reg& a1, Reg& a2, Reg& a3, Item* outbuf, Item* endA, Item* endB, Item** _endoutput, ui& exhaust) {
	// exhaust 0 -> both children have Items; 1 -> loadFrom empty; 2 -> opposite empty i.e. all empty
	constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
	Item* endoutput = *_endoutput;
	if (outbuf != endoutput && exhaust < 2) {
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;
		// run if both children have Items
		if (exhaust == 0) {
			while (outbuf != endoutput && loadFrom != endA && loadFrom != endB) {
				bool first = *loadFrom < *opposite;
				Item* tmp0 = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp0;
				origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);

				origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
				origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
				outbuf += INC;
				origami_utils::load<Reg>(a0, (Reg*)loadFrom);
				origami_utils::load<Reg>(a1, (Reg*)loadFrom + 1);
				_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
				loadFrom += INC;
			}
		}
		// if one of the streams run out
		if (outbuf != endoutput) {
			if (!exhaust) {
				origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);
				origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
				origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1); 
				outbuf += INC;
				exhaust = 1;
				//printf("Leaf exhausted\n");
			}
			Item* endOpposite = (loadFrom == endA) ? endB : endA;
			while (outbuf != endoutput && opposite != endOpposite) {
				origami_utils::load<Reg>(a0, (Reg*)opposite); 
				origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
				opposite += INC;
				origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);
				origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
				origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
				outbuf += INC;
			}
			if (outbuf != endoutput && exhaust != 2) {
				origami_utils::store<Reg, false>(a2, (Reg*)outbuf); 
				origami_utils::store<Reg, false>(a3, (Reg*)outbuf + 1); 
				outbuf += INC;
				exhaust = 2;
				endoutput = outbuf;		// we ran out of items so update end boundary
				//printf("Both leaf exhausted\n");
			}
		}
		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*_endoutput = endoutput;
	}
}

template <typename Reg, typename Item>
FORCEINLINE Item* merge_internal_to_internal(Item** _loadFrom, Item** _opposite, Reg& a0, Reg& a1, Reg& a2, Reg& a3, Item* outbuf, Item* endA, Item* endB, Item** _endoutput, ui exhaust0, ui exhaust1, ui& exhaust) {
	// exhaust 0 -> both children have keys; 1 -> loadFrom empty; 2 -> opposite empty i.e. all empty
	constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
	if (exhaust < 2) {
		Item* endoutput = *_endoutput;
		Item* loadFrom = *_loadFrom;
		Item* opposite = *_opposite;

		// run if both children have keys
		while (outbuf != endoutput && loadFrom != endA && loadFrom != endB) {
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			origami_utils::rswap<Reg, Item>(a0, a2);
			origami_utils::rswap<Reg, Item>(a1, a3);
			origami_utils::rswap<Reg, Item>(a1, a2);
			origami_utils::store<Reg, false>(a0, (Reg*)outbuf); 
			origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1); 
			outbuf += INC;
			//PAVX(a0);// PAVX(a1);
			origami_utils::load<Reg>(a0, (Reg*)loadFrom);
			origami_utils::load<Reg>(a1, (Reg*)loadFrom + 1);
			_mm_prefetch((char*)(loadFrom + 64), _MM_HINT_T2);
			loadFrom += INC;
		}

		// loadFrom exhausted
		if (outbuf != endoutput) {
			// this was the last batch from A
			if (exhaust0 == 2 && loadFrom == endA) {
				if (!exhaust) {
					origami_utils::rswap<Reg, Item>(a0, a2);
					origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
					origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
					outbuf += INC;
					exhaust = 1;
				}
				Item* endOpposite = endB;
				while (outbuf != endoutput && opposite != endOpposite) {
					origami_utils::load<Reg>(a0, (Reg*)opposite); 
					origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
					opposite += INC;
					origami_utils::rswap<Reg, Item>(a0, a2);
					origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
					origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
					outbuf += INC;
				}
				if (outbuf != endoutput) {
					if (exhaust1 == 2) {
						origami_utils::store<Reg, false>(a2, (Reg*)outbuf); 
						origami_utils::store<Reg, false>(a3, (Reg*)outbuf + 1); 
						outbuf += INC;
						exhaust = 2;
						endoutput = outbuf;
					}
					Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
				}

			}
			// or last batch from B
			else if (exhaust1 == 2 && loadFrom == endB) {
				if (!exhaust) {
					origami_utils::rswap<Reg, Item>(a0, a2);
					origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
					origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
					outbuf += INC;
					exhaust = 1;
				}
				Item* endOpposite = endA;
				while (outbuf != endoutput && opposite != endOpposite) {
					origami_utils::load<Reg>(a0, (Reg*)opposite); 
					origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
					opposite += INC;
					origami_utils::rswap<Reg, Item>(a0, a2);
					origami_utils::rswap<Reg, Item>(a1, a3);
					origami_utils::rswap<Reg, Item>(a1, a2);
					origami_utils::store<Reg, false>(a0, (Reg*)outbuf);
					origami_utils::store<Reg, false>(a1, (Reg*)outbuf + 1);
					outbuf += INC;
				}
				if (outbuf != endoutput) {
					if (exhaust0 == 2) {
						origami_utils::store<Reg, false>(a2, (Reg*)outbuf); 
						origami_utils::store<Reg, false>(a3, (Reg*)outbuf + 1); 
						outbuf += INC;
						exhaust = 2;
						endoutput = outbuf;
					}
					Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
				}
			}
		}

		*_loadFrom = loadFrom;
		*_opposite = opposite;
		*_endoutput = endoutput;
	}
	else *_endoutput = outbuf;
	return outbuf;
}

template <typename Reg, typename Item>
FORCEINLINE void merge_root_aligned(Item** _loadFrom, Item** _opposite, Reg& a2, Reg& a3, Item** _output, Item* endA, Item* endB, ui exhaust0, ui exhaust1) {
	constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
	Item* loadFrom = *_loadFrom;
	Item* opposite = *_opposite;
	Item* output = *_output;
	register Reg a0, a1;

	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	//printf("Exhaust0: %lu, Exhaust1: %lu\n", exhaust0, exhaust1); 

	while (loadFrom != endA && loadFrom != endB) {
		origami_utils::load<Reg>(a0, (Reg*)loadFrom); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom + 1); 
		loadFrom += INC;
		bool first = *loadFrom < *opposite;
		Item* tmp0 = first ? loadFrom : opposite;
		opposite = first ? opposite : loadFrom;
		loadFrom = tmp0;

		origami_utils::rswap<Reg, Item>(a0, a2);
		origami_utils::rswap<Reg, Item>(a1, a3);
		origami_utils::rswap<Reg, Item>(a1, a2);
		origami_utils::store<Reg, true>(a0, (Reg*)output);
		origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
		output += INC;
		//PAVX(a0); PAVX(a1);
	}
	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	// handle tail
	if ((exhaust0 == 2 && loadFrom == endA)) {
		//printf("Handling tail\n");
		Item* endOpposite = endB;
		while (opposite != endOpposite) {
			origami_utils::load<Reg>(a0, (Reg*)opposite); 
			origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
			opposite += INC;
			origami_utils::rswap<Reg, Item>(a0, a2);
			origami_utils::rswap<Reg, Item>(a1, a3);
			origami_utils::rswap<Reg, Item>(a1, a2);
			origami_utils::store<Reg, true>(a0, (Reg*)output);
			origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
			//PAVX(a0); PAVX(a1);
			output += INC;
		}
		if (exhaust1 == 2) {
			origami_utils::store<Reg, true>(a2, (Reg*)output); 
			origami_utils::store<Reg, true>(a3, (Reg*)output + 1); 
			//PAVX(a2); PAVX(a3);
			output += INC;
		}
		Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;	// std::swap is expensive
	}
	else if ((exhaust1 == 2 && loadFrom == endB)) {
		//printf("Handling tail\n");
		Item* endOpposite = endA;
		while (opposite != endOpposite) {
			origami_utils::load<Reg>(a0, (Reg*)opposite); 
			origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
			opposite += INC;
			origami_utils::rswap<Reg, Item>(a0, a2);
			origami_utils::rswap<Reg, Item>(a1, a3);
			origami_utils::rswap<Reg, Item>(a1, a2);
			origami_utils::store<Reg, true>(a0, (Reg*)output);
			origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
			//PAVX(a0); PAVX(a1);
			output += INC;
		}
		if (exhaust0 == 2) {
			origami_utils::store<Reg, true>(a2, (Reg*)output); 
			origami_utils::store<Reg, true>(a3, (Reg*)output + 1); 
			//PAVX(a2); PAVX(a3);
			output += INC;
		}
		Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
	}
	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	//origami_utils::store<Reg, true>(a0, (Reg*)output); output += INC;
	*_loadFrom = loadFrom;
	*_opposite = opposite;
	*_output = output;
}

template <typename Reg, typename Item>
FORCEINLINE void merge_root_unaligned(Item** _loadFrom, Item** _opposite, Reg& a2, Reg& a3, Item** _output, Item* endA, Item* endB, ui exhaust0, ui exhaust1, Item* outputEnd) {
	constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
	Item* loadFrom = *_loadFrom;
	Item* opposite = *_opposite;
	Item* output = *_output;
	register Reg a0, a1;

	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	//printf("Exhaust0: %lu, Exhaust1: %lu\n", exhaust0, exhaust1); 

	while (loadFrom != endA && loadFrom != endB && output != outputEnd) { // 
		origami_utils::load<Reg>(a0, (Reg*)loadFrom); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom + 1); 
		loadFrom += INC;
		bool first = *loadFrom < *opposite;
		Item* tmp0 = first ? loadFrom : opposite;
		opposite = first ? opposite : loadFrom;
		loadFrom = tmp0;

		origami_utils::rswap<Reg, Item>(a0, a2);
		origami_utils::rswap<Reg, Item>(a1, a3);
		origami_utils::rswap<Reg, Item>(a1, a2);
		origami_utils::store<Reg, true>(a0, (Reg*)output);
		origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
		output += INC;

		/*Item* newoutput = output + INC;
		ui prior_unalign_items_new = prior_unalign_items_loc - INC;
		output = prior_unalign_items_loc ? output : newoutput;
		prior_unalign_items_loc = prior_unalign_items_loc ? prior_unalign_items_new : prior_unalign_items_loc; */

	}

	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	// handle tail
	if (output != outputEnd) {
		if (exhaust0 == 2 && loadFrom == endA) {
			//printf("Handling tail\n");
			Item* endOpposite = endB;
			while (opposite != endOpposite && output != outputEnd) {
				origami_utils::load<Reg>(a0, (Reg*)opposite); 
				origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
				opposite += INC;
				origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);
				origami_utils::store<Reg, true>(a0, (Reg*)output);
				origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
				output += INC;
			}
			if (exhaust1 == 2 && output != outputEnd) {
				origami_utils::store<Reg, true>(a2, (Reg*)output); 
				origami_utils::store<Reg, true>(a3, (Reg*)output + 1); 
				output += INC;
			}
			Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;	// std::swap is expensive
		}
		else if (exhaust1 == 2 && loadFrom == endB) {
			//printf("Handling tail\n");
			Item* endOpposite = endA;
			while (opposite != endOpposite && output != outputEnd) {
				origami_utils::load<Reg>(a0, (Reg*)opposite); 
				origami_utils::load<Reg>(a1, (Reg*)opposite + 1); 
				opposite += INC;
				origami_utils::rswap<Reg, Item>(a0, a2);
				origami_utils::rswap<Reg, Item>(a1, a3);
				origami_utils::rswap<Reg, Item>(a1, a2);
				origami_utils::store<Reg, true>(a0, (Reg*)output);
				origami_utils::store<Reg, true>(a1, (Reg*)output + 1);
				output += INC;
			}
			if (exhaust0 == 2 && output != outputEnd) {
				origami_utils::store<Reg, true>(a2, (Reg*)output); 
				origami_utils::store<Reg, true>(a3, (Reg*)output + 1); 
				output += INC;
			}
			Item* tmp = loadFrom; loadFrom = opposite; opposite = tmp;
		}
	}
	//printf("LF: %llX, OP: %llX, ENDA: %llX, ENDB: %llX\n", loadFrom, opposite, endA, endB);
	//origami_utils::store<Reg, true>(a0, (Reg*)output); output += INC;
	*_loadFrom = loadFrom;
	*_opposite = opposite;
	*_output = output;
}

template <typename Reg, typename Item>
FORCEINLINE void merge_root_unaligned_skip_prior(Item** _loadFrom, Item** _opposite, Reg& a2, Reg& a3, Item* endA, Item* endB, ui exhaust0, ui exhaust1, ui* prior_unalign_items) {
	constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
	Item* loadFrom = *_loadFrom;
	Item* opposite = *_opposite;
	ui prior_unalign_items_loc = *prior_unalign_items;
	register Reg a0, a1;

	while (loadFrom != endA && loadFrom != endB && prior_unalign_items_loc > 0) { // 
		origami_utils::load<Reg>(a0, (Reg*)loadFrom); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom + 1); 
		loadFrom += INC;
		bool first = *loadFrom < *opposite;
		Item* tmp0 = first ? loadFrom : opposite;
		opposite = first ? opposite : loadFrom;
		loadFrom = tmp0;

		origami_utils::rswap<Reg, Item>(a0, a2);
		origami_utils::rswap<Reg, Item>(a1, a3);
		origami_utils::rswap<Reg, Item>(a1, a2);

		prior_unalign_items_loc -= INC;
	}

	*_loadFrom = loadFrom;
	*_opposite = opposite;
	*prior_unalign_items = prior_unalign_items_loc;
}

template <typename Reg, typename Item>
class Merge4Way {
public:
	Item* l1Buf0, * l1Buf1, * l1endBuf0, * l1endBuf1;
	Item* output, * outputEnd;
	Reg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11;
	Item* leafEnd0, * leafEnd1, * leafEnd2, * leafEnd3;
	Item* loadFrom0, * opposite0, * loadFrom1, * opposite1, * loadFrom, * opposite;
	ui exhaust0, exhaust1, exhaust2, exhaust3, exhaust4, exhaust5, exhaust;
	Item* l1endBuf0_back, * l1endBuf1_back, * outputEnd_back;

	void print_node() {
		printf("[%llX %llX]\n", output, outputEnd);
		printf("[%llX %llX] [%llX %llX]\n", l1Buf0, l1endBuf0, l1Buf1, l1endBuf1);
		printf("[%llX %llX] [%llX %llX] [%llX %llX] [%llX %llX]\n", loadFrom0, leafEnd0, opposite0, leafEnd1, loadFrom1, leafEnd2, opposite1, leafEnd3);
	}

	FORCEINLINE void merge_leaf_to_root_init() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;

		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a10 = this->a10;
		Reg a11 = this->a11;

		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); 
		loadFrom0 += INC;
		origami_utils::load<Reg>(a2, (Reg*)opposite0); 
		origami_utils::load<Reg>(a3, (Reg*)opposite0 + 1); 
		opposite0 += INC;
		origami_utils::load<Reg>(a4, (Reg*)loadFrom1); 
		origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1); 
		loadFrom1 += INC;
		origami_utils::load<Reg>(a6, (Reg*)opposite1); 
		origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1); 
		opposite1 += INC;

		irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
		irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);

		/*PRINT_ARR(l1Buf0, 32); PRINT_DASH(30);
		PRINT_ARR(l1Buf1, 32); PRINT_DASH(30);*/

		origami_utils::load<Reg>(a10, (Reg*)opposite); 
		origami_utils::load<Reg>(a11, (Reg*)opposite + 1); opposite += INC;

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a10 = a10;
		this->a11 = a11;

		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;
	}

	FORCEINLINE void merge_leaf_to_internal_init() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;

		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;

		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1); 
		loadFrom0 += INC;
		origami_utils::load<Reg>(a2, (Reg*)opposite0); 
		origami_utils::load<Reg>(a3, (Reg*)opposite0 + 1);
		opposite0 += INC;
		origami_utils::load<Reg>(a4, (Reg*)loadFrom1); 
		origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
		loadFrom1 += INC;
		origami_utils::load<Reg>(a6, (Reg*)opposite1); 
		origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1);
		opposite1 += INC;

		irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
		irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);


		origami_utils::load<Reg>(a8, (Reg*)loadFrom); 
		origami_utils::load<Reg>(a9, (Reg*)loadFrom + 1); loadFrom += INC;
		origami_utils::load<Reg>(a10, (Reg*)opposite); 
		origami_utils::load<Reg>(a11, (Reg*)opposite + 1); opposite += INC;

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;
	}

	FORCEINLINE void merge_internal_to_internal_init() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;

		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;

		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust0 = this->exhaust0;
		ui exhaust1 = this->exhaust1;
		ui exhaust2 = this->exhaust2;
		ui exhaust3 = this->exhaust3;
		ui exhaust4 = this->exhaust4;
		ui exhaust5 = this->exhaust5;

		origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
		loadFrom0 += INC;
		origami_utils::load<Reg>(a2, (Reg*)opposite0); 
		origami_utils::load<Reg>(a3, (Reg*)opposite0 + 1);
		opposite0 += INC;
		origami_utils::load<Reg>(a4, (Reg*)loadFrom1); 
		origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
		loadFrom1 += INC;
		origami_utils::load<Reg>(a6, (Reg*)opposite1); 
		origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1);
		opposite1 += INC;

		irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
		irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);


		origami_utils::load<Reg>(a8, (Reg*)loadFrom); 
		origami_utils::load<Reg>(a9, (Reg*)loadFrom + 1); loadFrom += INC;
		origami_utils::load<Reg>(a10, (Reg*)opposite); 
		origami_utils::load<Reg>(a11, (Reg*)opposite + 1); opposite += INC;

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;

		this->exhaust4 = exhaust4;
		this->exhaust5 = exhaust5;
	}

	FORCEINLINE void merge_internal_to_root_init() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;

		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a10 = this->a10;
		Reg a11 = this->a11;

		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust4 = this->exhaust4;
		ui exhaust5 = this->exhaust5;

		origami_utils::load<Reg>(a0, (Reg*)loadFrom0); 
		origami_utils::load<Reg>(a1, (Reg*)loadFrom0 + 1);
		loadFrom0 += INC;
		origami_utils::load<Reg>(a2, (Reg*)opposite0); 
		origami_utils::load<Reg>(a3, (Reg*)opposite0 + 1);
		opposite0 += INC;
		origami_utils::load<Reg>(a4, (Reg*)loadFrom1); 
		origami_utils::load<Reg>(a5, (Reg*)loadFrom1 + 1);
		loadFrom1 += INC;
		origami_utils::load<Reg>(a6, (Reg*)opposite1);
		origami_utils::load<Reg>(a7, (Reg*)opposite1 + 1);
		opposite1 += INC;

		irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
		irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);

		origami_utils::load<Reg>(a10, (Reg*)opposite); 
		origami_utils::load<Reg>(a11, (Reg*)opposite + 1); opposite += INC;

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a10 = a10;
		this->a11 = a11;

		this->exhaust4 = exhaust4;
		this->exhaust5 = exhaust5;
	}

	FORCEINLINE void merge_leaf_to_root() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;
		Item* output = this->output;
		Item* outputEnd = this->outputEnd;
		
		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;
		
		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		// merge
		
		while (output < outputEnd) {
			Item* o = output;
			merge_root_aligned<Reg, Item>(&loadFrom, &opposite, a10, a11, &output, l1endBuf0, l1endBuf1, exhaust0, exhaust1);
			/*printf("Merged: %llu\n", output - o);
			PRINT_ARR(o, (output - o)); PRINT_DASH(30);*/

			// refill empty child
			if (exhaust0 < 2 && loadFrom == l1endBuf0) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				loadFrom = l1Buf0;
			}
			else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
				loadFrom = l1Buf1;
			}
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;
		}
		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;

		this->output = output;
		this->outputEnd = outputEnd;
		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;
	}

	FORCEINLINE void merge_leaf_to_root_unaligned(ui* prior_unalign_items) {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;
		Item* output = this->output;
		Item* outputEnd = this->outputEnd;
		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;
		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		// merge

		// skip prior unaligned items
		while (1) {
			merge_root_unaligned_skip_prior(&loadFrom, &opposite, a10, a11, l1endBuf0, l1endBuf1, exhaust0, exhaust1, prior_unalign_items);
			if (*prior_unalign_items == 0) break;
			// refill empty child
			if (exhaust0 < 2 && loadFrom == l1endBuf0) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				loadFrom = l1Buf0;
			}
			else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
				loadFrom = l1Buf1;
			}
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;
		}


		Item* o = output;
		while (output < outputEnd) {
			merge_root_unaligned(&loadFrom, &opposite, a10, a11, &output, l1endBuf0, l1endBuf1, exhaust0, exhaust1, outputEnd);
			//printf("Merged: %llu\n", output - o);

			// refill empty child
			if (exhaust0 < 2 && loadFrom == l1endBuf0) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				loadFrom = l1Buf0;
			}
			else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
				loadFrom = l1Buf1;
			}
			bool first = *loadFrom < *opposite;
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;
		}
		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;

		this->output = output;
		this->outputEnd = outputEnd;
		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;
	}

	FORCEINLINE void merge_leaf_to_internal() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;

		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;

		Item* output = this->output;
		Item* outputEnd = this->outputEnd;

		//printf("Filling leaf to internal: [%llX %llX]\n", output, outputEnd);

		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;

		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust0 = this->exhaust0;
		ui exhaust1 = this->exhaust1;
		ui exhaust = this->exhaust;

		// merge
		ui64 cnt = 0;
		while (output < outputEnd) {
			//do {
				// refill empty child
			if (exhaust0 < 2 && loadFrom == l1endBuf0) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0);
				loadFrom = l1Buf0;
			}
			else if (exhaust1 < 2 && loadFrom == l1endBuf1) {
				irl_merge_tree::merge_leaf_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust1);
				loadFrom = l1Buf1;
			}
			bool first = *(loadFrom) < *(opposite);
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;
			//Item* o = output;
			output = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom, &opposite, a8, a9, a10, a11, output, l1endBuf0, l1endBuf1, &outputEnd, exhaust0, exhaust1, exhaust);
			//PRINT_ARR64(o, output - o); cnt += (output - o); printf("Tot: %llu\n", cnt);
		}
		//} while (output < outputEnd);
		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->exhaust0 = exhaust0;
		this->exhaust1 = exhaust1;
		this->exhaust = exhaust;

		this->outputEnd = outputEnd;
		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;
	}

	FORCEINLINE ui merge_internal_to_internal() {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;
		Item* output = this->output;
		Item* outputEnd = this->outputEnd;
		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;
		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust0 = this->exhaust0;
		ui exhaust1 = this->exhaust1;
		ui exhaust2 = this->exhaust2;
		ui exhaust3 = this->exhaust3;
		ui exhaust4 = this->exhaust4;
		ui exhaust5 = this->exhaust5;
		ui exhaust = this->exhaust;

		ui flag = 0;
		outputEnd = outputEnd_back;

		do {
			// refill empty child
			if (exhaust4 < 2 && loadFrom == l1endBuf0) {
				l1endBuf0 = l1endBuf0_back;
				l1endBuf0 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
				loadFrom = l1Buf0;
			}
			else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
				l1endBuf1 = l1endBuf1_back;
				l1endBuf1 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
				loadFrom = l1Buf1;
			}
			bool first = *(loadFrom) < *(opposite);
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			output = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom, &opposite, a8, a9, a10, a11, output, l1endBuf0, l1endBuf1, &outputEnd, exhaust4, exhaust5, exhaust);
			//PRINT_ARR(o, output - o);

			// check if any leaf node is empty
			ui empty0 = loadFrom0 == leafEnd0;
			ui empty1 = loadFrom0 == leafEnd1;
			ui empty2 = loadFrom1 == leafEnd2;
			ui empty3 = loadFrom1 == leafEnd3;
			flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
			//printf("Flag: %llx\n", flag);

		} while (flag == 0 && output < outputEnd);
		outputEnd = output;

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		//this->output = output;
		this->outputEnd = outputEnd;

		this->exhaust0 = exhaust0;
		this->exhaust1 = exhaust1;
		this->exhaust2 = exhaust2;
		this->exhaust3 = exhaust3;
		this->exhaust4 = exhaust4;
		this->exhaust5 = exhaust5;
		this->exhaust = exhaust;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;

		return flag;
	}

	FORCEINLINE ui merge_internal_to_root() {
		constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;
		Item* output = this->output;
		Item* outputEnd = this->outputEnd;
		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;
		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust0 = this->exhaust0;
		ui exhaust1 = this->exhaust1;
		ui exhaust2 = this->exhaust2;
		ui exhaust3 = this->exhaust3;
		ui exhaust4 = this->exhaust4;
		ui exhaust5 = this->exhaust5;
		ui exhaust = this->exhaust;
		// merge
		ui flag = 0;
		do {
			// refill empty child
			if (exhaust4 < 2 && loadFrom == l1endBuf0) {
				l1endBuf0 = l1endBuf0_back;
				l1endBuf0 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
				loadFrom = l1Buf0;
			}
			else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
				l1endBuf1 = l1endBuf1_back;
				l1endBuf1 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
				loadFrom = l1Buf1;
			}
			bool first = *(loadFrom) < *(opposite);
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			// from L1 to root

			Item* o = output;
			merge_root_aligned(&loadFrom, &opposite, a10, a11, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5);

			//PRINT_ARR(o, output - o);

			// check if any leaf node is empty
			ui empty0 = (loadFrom0 == leafEnd0);// && (exhaust0 < 2);
			ui empty1 = (loadFrom0 == leafEnd1);// && (exhaust1 < 2);
			ui empty2 = (loadFrom1 == leafEnd2);// && (exhaust2 < 2);
			ui empty3 = (loadFrom1 == leafEnd3);// && (exhaust3 < 2);
			flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
			//printf("Flag: %llx\n", flag);

		} while (flag == 0 && output < outputEnd);

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->output = output;
		this->outputEnd = outputEnd;

		this->exhaust0 = exhaust0;
		this->exhaust1 = exhaust1;
		this->exhaust2 = exhaust2;
		this->exhaust3 = exhaust3;
		this->exhaust4 = exhaust4;
		this->exhaust5 = exhaust5;
		this->exhaust = exhaust;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;

		return flag;
	}

	FORCEINLINE ui merge_internal_to_root_unaligned(ui* prior_unalign_items) {
		constexpr ui INC = sizeof(Reg) / sizeof(Item) * NREG;
		Item* loadFrom0 = this->loadFrom0;
		Item* loadFrom1 = this->loadFrom1;
		Item* loadFrom = this->loadFrom;
		Item* opposite0 = this->opposite0;
		Item* opposite1 = this->opposite1;
		Item* opposite = this->opposite;
		Item* l1Buf0 = this->l1Buf0;
		Item* l1Buf1 = this->l1Buf1;
		Item* l1endBuf0 = this->l1endBuf0;
		Item* l1endBuf1 = this->l1endBuf1;
		Item* output = this->output;
		Item* outputEnd = this->outputEnd;
		Reg a0 = this->a0;
		Reg a1 = this->a1;
		Reg a2 = this->a2;
		Reg a3 = this->a3;
		Reg a5 = this->a5;
		Reg a6 = this->a6;
		Reg a7 = this->a7;
		Reg a8 = this->a8;
		Reg a9 = this->a9;
		Reg a10 = this->a10;
		Reg a11 = this->a11;
		Item* leafEnd0 = this->leafEnd0;
		Item* leafEnd1 = this->leafEnd1;
		Item* leafEnd2 = this->leafEnd2;
		Item* leafEnd3 = this->leafEnd3;

		ui exhaust0 = this->exhaust0;
		ui exhaust1 = this->exhaust1;
		ui exhaust2 = this->exhaust2;
		ui exhaust3 = this->exhaust3;
		ui exhaust4 = this->exhaust4;
		ui exhaust5 = this->exhaust5;
		ui exhaust = this->exhaust;

		// merge

		ui flag = 0;
		// skip over prior partition keys
		do {
			// refill empty child
			if (exhaust4 < 2 && loadFrom == l1endBuf0) {
				l1endBuf0 = l1endBuf0_back;
				l1endBuf0 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
				loadFrom = l1Buf0;
			}
			else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
				l1endBuf1 = l1endBuf1_back;
				l1endBuf1 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
				loadFrom = l1Buf1;
			}
			bool first = *(loadFrom) < *(opposite);
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			Item* o = output;
			merge_root_unaligned_skip_prior(&loadFrom, &opposite, a10, a11, l1endBuf0, l1endBuf1, exhaust4, exhaust5, prior_unalign_items);

			// check if any leaf node is empty
			ui empty0 = (loadFrom0 == leafEnd0);
			ui empty1 = (loadFrom0 == leafEnd1);
			ui empty2 = (loadFrom1 == leafEnd2);
			ui empty3 = (loadFrom1 == leafEnd3);
			flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
			//printf("Flag: %llx\n", flag);

		} while (flag == 0 && *prior_unalign_items > 0);

		// previous parition keys skipped
		do {
			// refill empty child
			if (exhaust4 < 2 && loadFrom == l1endBuf0) {
				l1endBuf0 = l1endBuf0_back;
				l1endBuf0 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom0, &opposite0, a0, a1, a2, a3, l1Buf0, leafEnd0, leafEnd1, &l1endBuf0, exhaust0, exhaust1, exhaust4);
				loadFrom = l1Buf0;
			}
			else if (exhaust5 < 2 && loadFrom == l1endBuf1) {
				l1endBuf1 = l1endBuf1_back;
				l1endBuf1 = irl_merge_tree::merge_internal_to_internal<Reg, Item>(&loadFrom1, &opposite1, a4, a5, a6, a7, l1Buf1, leafEnd2, leafEnd3, &l1endBuf1, exhaust2, exhaust3, exhaust5);
				loadFrom = l1Buf1;
			}
			bool first = *(loadFrom) < *(opposite);
			Item* tmp0 = first ? loadFrom : opposite;
			opposite = first ? opposite : loadFrom;
			loadFrom = tmp0;

			// from L1 to root

			Item* o = output;
			merge_root_unaligned(&loadFrom, &opposite, a10, a11, &output, l1endBuf0, l1endBuf1, exhaust4, exhaust5, outputEnd);

			// check if any leaf node is empty
			ui empty0 = (loadFrom0 == leafEnd0);// && (exhaust0 < 2);
			ui empty1 = (loadFrom0 == leafEnd1);// && (exhaust1 < 2);
			ui empty2 = (loadFrom1 == leafEnd2);// && (exhaust2 < 2);
			ui empty3 = (loadFrom1 == leafEnd3);// && (exhaust3 < 2);
			flag = empty0 | (empty1 << 1) | (empty2 << 2) | (empty3 << 3);
			//printf("Flag: %llx\n", flag);

		} while (flag == 0 && output < outputEnd);

		this->loadFrom0 = loadFrom0;
		this->loadFrom1 = loadFrom1;
		this->loadFrom = loadFrom;
		this->opposite0 = opposite0;
		this->opposite1 = opposite1;
		this->opposite = opposite;

		this->l1Buf0 = l1Buf0;
		this->l1Buf1 = l1Buf1;
		this->l1endBuf0 = l1endBuf0;
		this->l1endBuf1 = l1endBuf1;

		this->output = output;
		this->outputEnd = outputEnd;

		this->exhaust0 = exhaust0;
		this->exhaust1 = exhaust1;
		this->exhaust2 = exhaust2;
		this->exhaust3 = exhaust3;
		this->exhaust4 = exhaust4;
		this->exhaust5 = exhaust5;
		this->exhaust = exhaust;

		this->a0 = a0;
		this->a1 = a1;
		this->a2 = a2;
		this->a3 = a3;
		this->a4 = a4;
		this->a5 = a5;
		this->a6 = a6;
		this->a7 = a7;
		this->a8 = a8;
		this->a9 = a9;
		this->a10 = a10;
		this->a11 = a11;

		return flag;
	}

	FORCEINLINE void initialize(Item* _leafBuf0, Item* _leafBuf1, Item* _leafBuf2, Item* _leafBuf3,
		Item* _leafEnd0, Item* _leafEnd1, Item* _leafEnd2, Item* _leafEnd3,
		Item* _l1Buf0, Item* _l1Buf1, Item* _l1endBuf0, Item* _l1endBuf1,
		Item* _output, Item* _outputEnd) {

		loadFrom0 = _leafBuf0;	opposite0 = _leafBuf1;  loadFrom1 = _leafBuf2;  opposite1 = _leafBuf3;
		leafEnd0 = _leafEnd0;	leafEnd1 = _leafEnd1; 	leafEnd2 = _leafEnd2; 	leafEnd3 = _leafEnd3;

		l1Buf0 = _l1Buf0;	l1Buf1 = _l1Buf1; 	l1endBuf0 = _l1endBuf0; l1endBuf1 = _l1endBuf1;
		l1endBuf0_back = _l1endBuf0; l1endBuf1_back = _l1endBuf1;

		loadFrom = l1Buf0; opposite = l1Buf1;

		output = _output; outputEnd = _outputEnd; outputEnd_back = _outputEnd;

		exhaust0 = exhaust1 = exhaust2 = exhaust3 = exhaust4 = exhaust5 = exhaust = 0;
	}
};
#endif


	template <typename Reg, typename Item, bool external = false>
	void RefillNode(Merge4Way<Reg, Item, external>** nodes, ui node_idx, ui level, ui LEVELS_4WAY, ui leaf_base = 0, ui buff_base = 0, origami_utils::IOHelper* IO = nullptr) {
		if (level == (LEVELS_4WAY - 1)) {		// leaf level
			ui buf_idx = buff_base + ((node_idx - leaf_base) << 2);
			//printf("Node idx: %lu, Buff base: %lu, Buf idx: %lu, Leaf base: %lu\n", node_idx, buff_base, buf_idx, leaf_base);
			nodes[node_idx]->merge_leaf_to_internal(buf_idx, IO);
			return;
		}

		ui empty = nodes[node_idx]->merge_internal_to_internal();

		Item* _loadFrom0 = nodes[node_idx]->loadFrom0;
		Item* _loadFrom1 = nodes[node_idx]->loadFrom1;
		Item* _opposite0 = nodes[node_idx]->opposite0;
		Item* _opposite1 = nodes[node_idx]->opposite1;

		ui c_idx_base = (node_idx << 2) + 1;
		ui j = 0;
		while (j < 2) {
			ui c_idx = c_idx_base + j;
			if (((empty >> j) & 1) && (nodes[c_idx]->exhaust < 2)) {
				RefillNode(nodes, c_idx, level + 1, LEVELS_4WAY, leaf_base, buff_base, IO);
				_loadFrom0 = nodes[c_idx]->output;

				bool first = *_loadFrom0 < *_opposite0;
				Item* tmp0 = first ? _loadFrom0 : _opposite0;
				_opposite0 = first ? _opposite0 : _loadFrom0;
				_loadFrom0 = tmp0;
			}
			++j;
		}
		while (j < 4) {
			ui c_idx = c_idx_base + j;
			if (((empty >> j) & 1) && (nodes[c_idx]->exhaust < 2)) {
				RefillNode(nodes, c_idx, level + 1, LEVELS_4WAY, leaf_base, buff_base, IO);
				_loadFrom1 = nodes[c_idx]->output;

				bool first = *_loadFrom1 < *_opposite1;
				Item* tmp0 = first ? _loadFrom1 : _opposite1;
				_opposite1 = first ? _opposite1 : _loadFrom1;
				_loadFrom1 = tmp0;
			}
			++j;
		}
		nodes[node_idx]->exhaust0 = nodes[c_idx_base]->exhaust;			nodes[node_idx]->leafEnd0 = nodes[c_idx_base]->outputEnd;
		nodes[node_idx]->exhaust1 = nodes[c_idx_base + 1]->exhaust;		nodes[node_idx]->leafEnd1 = nodes[c_idx_base + 1]->outputEnd;
		nodes[node_idx]->exhaust2 = nodes[c_idx_base + 2]->exhaust;		nodes[node_idx]->leafEnd2 = nodes[c_idx_base + 2]->outputEnd;
		nodes[node_idx]->exhaust3 = nodes[c_idx_base + 3]->exhaust;		nodes[node_idx]->leafEnd3 = nodes[c_idx_base + 3]->outputEnd;

		nodes[node_idx]->loadFrom0 = _loadFrom0;
		nodes[node_idx]->loadFrom1 = _loadFrom1;
		nodes[node_idx]->opposite0 = _opposite0;
		nodes[node_idx]->opposite1 = _opposite1;
	}

	template <typename Reg, typename Item, bool external = false>
	class MergeTree {
	public:
		Merge4Way<Reg, Item, external>** nodes = nullptr;
		Item* bufptrs[MTREE_MAX_LEVEL * MTREE_MAX_WAY], * bufptrsEnd[MTREE_MAX_LEVEL * MTREE_MAX_WAY];
		ui LEVELS, LEAF_LEVEL, ROOT_LEVEL = 0;
		ui LEVELS_4WAY, NODES;
		ui prior_unalign_items = 0, post_unalign_items = 0;
		origami_utils::IOHelper* IO = nullptr;

		FORCEINLINE virtual void merge_init(ui WAY, Item* buf, ui buf_n, ui l2_buf_n, origami_utils::IOHelper* _IO = nullptr) = 0;
		FORCEINLINE virtual void merge_cleanup() = 0;
		FORCEINLINE virtual void merge(Item* A, Item* C, ui64 chunk, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) = 0;
		FORCEINLINE virtual void merge(Item** _X, Item** _endX, Item* C, ui64 n, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) = 0;
	};

	template <typename Reg, typename Item, bool external = false>
	class MergeTreeEven : public MergeTree<Reg, Item, external> {
	public:
		FORCEINLINE void merge_init(ui WAY, Item* buf, ui buf_n, ui l2_buf_n, origami_utils::IOHelper* _IO = nullptr) {
			this->LEVELS = (ui)(log2(WAY)) + 1;
			this->LEAF_LEVEL = this->LEVELS - 1;
			this->LEVELS_4WAY = this->LEVELS >> 1;
			this->NODES = 0;
			FOR(i, this->LEVELS_4WAY, 1) this->NODES += pow(4, i);
			this->nodes = new Merge4Way<Reg, Item, external> * [this->NODES];
			FOR(i, this->NODES, 1) this->nodes[i] = new Merge4Way<Reg, Item, external>();

			// init interim buffer start and end pointers
			Item* p = buf;
			FOR_INIT(i, 1, this->LEAF_LEVEL, 1) {
				ui nodes_at_level = 1U << i;
				ui level_offset = i * WAY;
				Item** _bufptr = this->bufptrs + level_offset;
				Item** _bufptrEnd = this->bufptrsEnd + level_offset;
				// for large l2 buffers
				if ((i & 1) == 0) {	// level 2, 4 and so on
					FOR(j, nodes_at_level, 1) {
						*_bufptr = p; _bufptr++;
						*_bufptrEnd = p + l2_buf_n; _bufptrEnd++;
						p += l2_buf_n + (64 / sizeof(Item));	
					}
				}
				else {
					FOR(j, nodes_at_level, 1) {
						*_bufptr = p; _bufptr++;
						*_bufptrEnd = p + buf_n; _bufptrEnd++;
						p += buf_n + (64 / sizeof(Item));	
					}
				}
			}

			this->IO = _IO;
		}

		FORCEINLINE void merge_cleanup() {
			FOR(i, this->NODES, 1) delete this->nodes[i];
			delete[] this->nodes;
		}

		FORCEINLINE void merge(Item* A, Item* C, ui64 chunk, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) {
			this->prior_unalign_items = 0;
			this->post_unalign_items = 0;
			//printf("Merging: [%llX %llX] to [%llX %llX], Tot: %llu\n", A, A + chunk * WAY, C, C + chunk * WAY, chunk * WAY);
			// create local vars
			Merge4Way<Reg, Item, external>** nodes = this->nodes;
			Item** bufptrs = this->bufptrs, ** bufptrsEnd = this->bufptrsEnd;
			ui LEAF_LEVEL = this->LEAF_LEVEL;

			// initialization begin 
			// 1. init leaf pointers --> only leaf ptrs and output ptrs will change for each merge
			ui leaf_level_offset = LEAF_LEVEL * WAY;
			Item** _bufptr = bufptrs + leaf_level_offset;
			Item** _bufptrEnd = bufptrsEnd + leaf_level_offset;
			FOR(i, WAY, 1) {
				Item* p = A + i * chunk;
				*_bufptr = p;
				*_bufptrEnd = p + chunk;
				_bufptr++; _bufptrEnd++;
			}

			// 2. init root ptr as output buffer
			Item* output = C, * outputEnd = C + WAY * chunk;
			bufptrs[0] = output;
			bufptrsEnd[0] = outputEnd;

			ui idx_offset = 0; // NODES - 1;
			FOR(curr_level, LEAF_LEVEL, 2) {
				ui nodes_at_level = 1 << curr_level;
				FOR(i, nodes_at_level, 1) {
					ui node_idx = curr_level * WAY + i;
					ui lidx = (curr_level + 1) * WAY + (i << 1); ui lidx2 = (curr_level + 2) * WAY + (i << 2);
					nodes[idx_offset]->initialize(bufptrs[lidx2], bufptrs[lidx2 + 1], bufptrs[lidx2 + 2], bufptrs[lidx2 + 3],
						bufptrsEnd[lidx2], bufptrsEnd[lidx2 + 1], bufptrsEnd[lidx2 + 2], bufptrsEnd[lidx2 + 3],
						bufptrs[lidx], bufptrs[lidx + 1], bufptrsEnd[lidx], bufptrsEnd[lidx + 1],
						bufptrs[node_idx], bufptrsEnd[node_idx]);
					++idx_offset;
				}
			}

			// Initialization
			if (this->LEVELS_4WAY == 1)
				nodes[0]->merge_leaf_to_root_init(0);
			else if (this->LEVELS_4WAY == 2) {
				FOR_INIT(i, 1, 5, 1) {
					nodes[i]->merge_leaf_to_internal_init((i - 1) << 2);
					nodes[i]->merge_leaf_to_internal();
					//printf("Internal node contains: %llu; Exhaust: %lu\n", nodes[i]->outputEnd - nodes[i]->output, nodes[i]->exhaust);
				}
				// to handle the case when initilization empties out these nodes ** NOTE: need to add this to the following else condition
				nodes[0]->exhaust0 = nodes[1]->exhaust;	nodes[0]->leafEnd0 = nodes[1]->outputEnd;
				nodes[0]->exhaust1 = nodes[2]->exhaust;	nodes[0]->leafEnd1 = nodes[2]->outputEnd;
				nodes[0]->exhaust2 = nodes[3]->exhaust;	nodes[0]->leafEnd2 = nodes[3]->outputEnd;
				nodes[0]->exhaust3 = nodes[4]->exhaust;	nodes[0]->leafEnd3 = nodes[4]->outputEnd;
				nodes[0]->merge_internal_to_root_init();
			}
			else {
				ui nodes_last_level = 1LU << ((this->LEVELS_4WAY - 1) << 1);
				ui idx = this->NODES - 1;
				FOR(i, nodes_last_level, 1) {
					nodes[idx]->merge_leaf_to_internal_init();
					nodes[idx]->merge_leaf_to_internal();
					ui pidx = (idx - 1) >> 2;
					ui exhaust = nodes[idx]->exhaust;
					Item* _outputEnd = nodes[idx]->outputEnd;
					switch (idx & 3) {
					case 1:
						nodes[pidx]->exhaust0 = exhaust;
						nodes[pidx]->leafEnd0 = _outputEnd;
						break;
					case 2:
						nodes[pidx]->exhaust1 = exhaust;
						nodes[pidx]->leafEnd1 = _outputEnd;
						break;
					case 3:
						nodes[pidx]->exhaust2 = exhaust;
						nodes[pidx]->leafEnd2 = _outputEnd;
						break;
					case 0:	// last node
						nodes[pidx]->exhaust3 = exhaust;
						nodes[pidx]->leafEnd3 = _outputEnd;
						break;
					}
					--idx;
				}
				for (int i = this->LEVELS_4WAY - 2; i > 0; --i) {
					ui nodes_level = 1LU << (i << 1);
					FOR(j, nodes_level, 1) {
						nodes[idx]->merge_internal_to_internal_init();
						nodes[idx]->merge_internal_to_internal();
						ui pidx = (idx - 1) >> 2;
						ui exhaust = nodes[idx]->exhaust;
						Item* _outputEnd = nodes[idx]->outputEnd;
						switch (idx & 3) {
						case 1:
							nodes[pidx]->exhaust0 = exhaust;
							nodes[pidx]->leafEnd0 = _outputEnd;
							break;
						case 2:
							nodes[pidx]->exhaust1 = exhaust;
							nodes[pidx]->leafEnd1 = _outputEnd;
							break;
						case 3:
							nodes[pidx]->exhaust2 = exhaust;
							nodes[pidx]->leafEnd2 = _outputEnd;
							break;
						case 0:	// last node
							nodes[pidx]->exhaust3 = exhaust;
							nodes[pidx]->leafEnd3 = _outputEnd;
							break;
						}
						--idx;
					}
				}
				nodes[0]->merge_internal_to_root_init();
			}

			// Merge 
			if (this->LEVELS_4WAY == 1) {
				nodes[0]->merge_leaf_to_root();
				return;
			}
			ui empty = 0;
			while (nodes[0]->output < outputEnd) {
				Item* o = nodes[0]->output;
				empty = nodes[0]->merge_internal_to_root();
				Item* oNew = nodes[0]->output;
				//PRINT_ARR(o, oNew - o);

				Item* loadFrom0 = nodes[0]->loadFrom0;
				Item* loadFrom1 = nodes[0]->loadFrom1;
				Item* opposite0 = nodes[0]->opposite0;
				Item* opposite1 = nodes[0]->opposite1;

				ui i = 0;
				while (i < 2) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode(nodes, cidx, 1, this->LEVELS_4WAY);
						loadFrom0 = nodes[cidx]->output;

						bool first = *loadFrom0 < *opposite0;
						Item* tmp0 = first ? loadFrom0 : opposite0;
						opposite0 = first ? opposite0 : loadFrom0;
						loadFrom0 = tmp0;
					}
					++i;
				}
				while (i < 4) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode<Reg, Item>(nodes, cidx, 1, this->LEVELS_4WAY);
						loadFrom1 = nodes[cidx]->output;

						bool first = *loadFrom1 < *opposite1;
						Item* tmp0 = first ? loadFrom1 : opposite1;
						opposite1 = first ? opposite1 : loadFrom1;
						loadFrom1 = tmp0;
					}
					++i;
				}

				nodes[0]->exhaust0 = nodes[1]->exhaust;	nodes[0]->leafEnd0 = nodes[1]->outputEnd;
				nodes[0]->exhaust1 = nodes[2]->exhaust;	nodes[0]->leafEnd1 = nodes[2]->outputEnd;
				nodes[0]->exhaust2 = nodes[3]->exhaust;	nodes[0]->leafEnd2 = nodes[3]->outputEnd;
				nodes[0]->exhaust3 = nodes[4]->exhaust;	nodes[0]->leafEnd3 = nodes[4]->outputEnd;

				nodes[0]->loadFrom0 = loadFrom0;
				nodes[0]->loadFrom1 = loadFrom1;
				nodes[0]->opposite0 = opposite0;
				nodes[0]->opposite1 = opposite1;
			}
		}

		FORCEINLINE void merge(Item** _X, Item** _endX, Item* C, ui64 n, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) {
			// debug verify correctness of partitioning
			/*Item* O = C;
			FOR(i, WAY, 1) {
				memcpy(O, _X[i], (_endX[i] - _X[i]) * sizeof(Item));
				O += (_endX[i] - _X[i]);
			}
			std::sort(C, C + n);
			return; */

			// debug: check if lists sorted
			/*FOR(i, WAY, 1) {
				printf("Checking stream %u w/ %llu items ... ", i, (_endX[i] - _X[i]));
				SortCorrectnessChecker<Item>(_X[i], (_endX[i] - _X[i]));
				printf("done\n");
			}
			return;*/


			constexpr ui ITEMS_PER_REG = sizeof(Reg) / sizeof(Item);
			this->prior_unalign_items = 0;
			this->post_unalign_items = 0;
			// create local vars
			Merge4Way<Reg, Item, external>** nodes = this->nodes;
			Item** bufptrs = this->bufptrs, ** bufptrsEnd = this->bufptrsEnd;
			ui LEAF_LEVEL = this->LEAF_LEVEL;

			ui64 n_temp = 0;
			C = (Item*)(origami_utils::align<Reg, MTREE_NREG>((char*)C));

			// initialization begin 
			// 1. init leaf pointers

			ui leaf_level_offset = LEAF_LEVEL * WAY;
			Item** _bufptr = bufptrs + leaf_level_offset;
			Item** _bufptrEnd = bufptrsEnd + leaf_level_offset;
			FOR(i, WAY, 1) {
				//printf("[%llX %llX]\n", _X[i], _endX[i]);
				Item* x = _X[i];
				Item* aligned_x = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)x);
				Item* endx = _endX[i];
				bool endx_aligned = origami_utils::aligned<Reg, MTREE_NREG>((char*)endx);
				Item* endx2 = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)endx) + ITEMS_PER_REG;
				Item* aligned_endx = endx_aligned ? endx : endx2;

				//printf("Prior unalign: %lu, Post unalign: %lu\n", x - aligned_x, aligned_endx - endx);
				//printf("[%llX %llX], [%llX %llX]\n", x, aligned_x, endx, aligned_endx);
				this->prior_unalign_items += x - aligned_x;
				this->post_unalign_items += aligned_endx - endx;

				*_bufptr = aligned_x;
				*_bufptrEnd = aligned_endx;
				n_temp += (endx - x);
				_bufptr++; _bufptrEnd++;
			}
			//n = n_temp;

			/*printf("Tot prior unalign items: %lu, post unalign items: %lu\n", prior_unalign_items, post_unalign_items);
			printf("N: %lu\n", n);*/

		

			// 2. init root ptr as output buffer
			Item* output = C, * outputEnd = C + n;
			bufptrs[0] = output;
			bufptrsEnd[0] = outputEnd;

			ui idx_offset = 0; // NODES - 1;
			//for (int curr_level = LEAF_LEVEL - 2; curr_level >= 0; curr_level -= 2) {
			FOR(curr_level, LEAF_LEVEL, 2) {
				ui nodes_at_level = 1 << curr_level;
				FOR(i, nodes_at_level, 1) {
					ui node_idx = curr_level * WAY + i;
					ui lidx = (curr_level + 1) * WAY + (i << 1); ui lidx2 = (curr_level + 2) * WAY + (i << 2);
					nodes[idx_offset]->initialize(bufptrs[lidx2], bufptrs[lidx2 + 1], bufptrs[lidx2 + 2], bufptrs[lidx2 + 3],
						bufptrsEnd[lidx2], bufptrsEnd[lidx2 + 1], bufptrsEnd[lidx2 + 2], bufptrsEnd[lidx2 + 3],
						bufptrs[lidx], bufptrs[lidx + 1], bufptrsEnd[lidx], bufptrsEnd[lidx + 1],
						bufptrs[node_idx], bufptrsEnd[node_idx]);
					++idx_offset;
				}
			}

			// Initialization
			if (this->LEVELS_4WAY == 1)
				nodes[0]->merge_leaf_to_root_init(0, this->IO);
			else if (this->LEVELS_4WAY == 2) {
				FOR_INIT(i, 1, 5, 1) {
					nodes[i]->merge_leaf_to_internal_init((i - 1) << 2, this->IO);
					nodes[i]->merge_leaf_to_internal((i - 1) << 2, this->IO);
					//printf("Internal node %lu contains: %llu; Exhaust: %lu\n", i, nodes[i]->outputEnd - nodes[i]->output, nodes[i]->exhaust);
				}
				// to handle the case when initilization empties out these nodes ** NOTE: need to add this to the following else condition
				nodes[0]->exhaust0 = nodes[1]->exhaust;	nodes[0]->leafEnd0 = nodes[1]->outputEnd;
				nodes[0]->exhaust1 = nodes[2]->exhaust;	nodes[0]->leafEnd1 = nodes[2]->outputEnd;
				nodes[0]->exhaust2 = nodes[3]->exhaust;	nodes[0]->leafEnd2 = nodes[3]->outputEnd;
				nodes[0]->exhaust3 = nodes[4]->exhaust;	nodes[0]->leafEnd3 = nodes[4]->outputEnd;
				nodes[0]->merge_internal_to_root_init();
			}
			else {
				ui nodes_last_level = 1LU << ((this->LEVELS_4WAY - 1) << 1);
				ui idx = this->NODES - 1;
				ui leaf_base = this->NODES - (WAY >> 2);
				FOR(i, nodes_last_level, 1) {
					ui buf_idx = (idx - leaf_base) << 2;
					nodes[idx]->merge_leaf_to_internal_init(buf_idx, this->IO);
					nodes[idx]->merge_leaf_to_internal(buf_idx, this->IO);
					ui pidx = (idx - 1) >> 2;
					ui exhaust = nodes[idx]->exhaust;
					Item* _outputEnd = nodes[idx]->outputEnd;
					switch (idx & 3) {
					case 1:
						nodes[pidx]->exhaust0 = exhaust;
						nodes[pidx]->leafEnd0 = _outputEnd;
						break;
					case 2:
						nodes[pidx]->exhaust1 = exhaust;
						nodes[pidx]->leafEnd1 = _outputEnd;
						break;
					case 3:
						nodes[pidx]->exhaust2 = exhaust;
						nodes[pidx]->leafEnd2 = _outputEnd;
						break;
					case 0:	// last node
						nodes[pidx]->exhaust3 = exhaust;
						nodes[pidx]->leafEnd3 = _outputEnd;
						break;
					}
					--idx;
				}
				for (int i = this->LEVELS_4WAY - 2; i > 0; --i) {
					ui nodes_level = 1LU << (i << 1);
					FOR(j, nodes_level, 1) {
						nodes[idx]->merge_internal_to_internal_init();
						nodes[idx]->merge_internal_to_internal();
						ui pidx = (idx - 1) >> 2;
						ui exhaust = nodes[idx]->exhaust;
						Item* _outputEnd = nodes[idx]->outputEnd;
						switch (idx & 3) {
						case 1:
							nodes[pidx]->exhaust0 = exhaust;
							nodes[pidx]->leafEnd0 = _outputEnd;
							break;
						case 2:
							nodes[pidx]->exhaust1 = exhaust;
							nodes[pidx]->leafEnd1 = _outputEnd;
							break;
						case 3:
							nodes[pidx]->exhaust2 = exhaust;
							nodes[pidx]->leafEnd2 = _outputEnd;
							break;
						case 0:	// last node
							nodes[pidx]->exhaust3 = exhaust;
							nodes[pidx]->leafEnd3 = _outputEnd;
							break;
						}
						--idx;
					}
				}
				nodes[0]->merge_internal_to_root_init();
			}


			// Merge 
			if (this->LEVELS_4WAY == 1) {
				nodes[0]->merge_leaf_to_root_unaligned(&this->prior_unalign_items, this->IO);
				return;
			}
			ui empty = 0;
			ui tot = 0;
			//ui lcnt = 0;
			while (nodes[0]->output < outputEnd) {
				empty = nodes[0]->merge_internal_to_root_unaligned(&this->prior_unalign_items, this->IO);

				Item* loadFrom0 = nodes[0]->loadFrom0;
				Item* loadFrom1 = nodes[0]->loadFrom1;
				Item* opposite0 = nodes[0]->opposite0;
				Item* opposite1 = nodes[0]->opposite1;

				ui i = 0;
				while (i < 2) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode<Reg, Item>(nodes, cidx, 1, this->LEVELS_4WAY, this->NODES - (WAY >> 2), 0, this->IO);
						loadFrom0 = nodes[cidx]->output;

						bool first = *loadFrom0 < *opposite0;
						Item* tmp0 = first ? loadFrom0 : opposite0;
						opposite0 = first ? opposite0 : loadFrom0;
						loadFrom0 = tmp0;
					}
					++i;
				}
				while (i < 4) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode<Reg, Item>(nodes, cidx, 1, this->LEVELS_4WAY, this->NODES - (WAY >> 2), 0, this->IO);
						loadFrom1 = nodes[cidx]->output;

						bool first = *loadFrom1 < *opposite1;
						Item* tmp0 = first ? loadFrom1 : opposite1;
						opposite1 = first ? opposite1 : loadFrom1;
						loadFrom1 = tmp0;
					}
					++i;
				}

				nodes[0]->exhaust0 = nodes[1]->exhaust;	nodes[0]->leafEnd0 = nodes[1]->outputEnd;
				nodes[0]->exhaust1 = nodes[2]->exhaust;	nodes[0]->leafEnd1 = nodes[2]->outputEnd;
				nodes[0]->exhaust2 = nodes[3]->exhaust;	nodes[0]->leafEnd2 = nodes[3]->outputEnd;
				nodes[0]->exhaust3 = nodes[4]->exhaust;	nodes[0]->leafEnd3 = nodes[4]->outputEnd;

				nodes[0]->loadFrom0 = loadFrom0;
				nodes[0]->loadFrom1 = loadFrom1;
				nodes[0]->opposite0 = opposite0;
				nodes[0]->opposite1 = opposite1;
			}
		}
	};
	// for merge-tree that is not power of 4-way (32-way, 128-way etc.)

	template <typename Reg, typename Item, bool external = false>
	class MergeTreeOdd : public MergeTree<Reg, Item, external> {
	public:
		//Merge4Way<Reg, Item>** nodes_left = nullptr, ** nodes_right = nullptr;
		//Item* bufptrs1[MAX_LEVEL * MAX_WAY], * bufptrsEnd1[MAX_LEVEL * MAX_WAY];
		Merge4Way<Reg, Item, external>** nodes_right = nullptr;
		Item* bufptrs2[MTREE_MAX_LEVEL * MTREE_MAX_WAY], * bufptrsEnd2[MTREE_MAX_LEVEL * MTREE_MAX_WAY];
		//ui LEVELS, LEAF_LEVEL, ROOT_LEVEL = 0;
		//ui LEVELS_4WAY, NODES;

		FORCEINLINE void merge_init(ui WAY, Item* buf, ui buf_n, ui l2_buf_n, origami_utils::IOHelper* _IO = nullptr) override {
			Merge4Way<Reg, Item, external>** nodes_left = this->nodes;
			Item** bufptrs1 = this->bufptrs, ** bufptrsEnd1 = this->bufptrsEnd;
			const ui HALF_WAY = WAY >> 1;
			this->LEVELS = (ui)(log2(HALF_WAY)) + 1;
			this->LEAF_LEVEL = this->LEVELS - 1;
			this->LEVELS_4WAY = this->LEVELS >> 1;
			this->NODES = 0;
			FOR(i, this->LEVELS_4WAY, 1) this->NODES += pow(4, i);
			nodes_left = new Merge4Way<Reg, Item, external> * [this->NODES];
			nodes_right = new Merge4Way<Reg, Item, external> * [this->NODES];
			FOR(i, this->NODES, 1) {
				nodes_left[i] = new Merge4Way<Reg, Item, external>();
				nodes_right[i] = new Merge4Way<Reg, Item, external>();
			}

			// init interim buffer start and end pointers
			Item* p1 = buf;		// NOTE: need to fix this later
			FOR_INIT(i, 1, this->LEAF_LEVEL, 1) {
				ui nodes_at_level = 1U << i;
				ui level_offset = i * HALF_WAY;
				Item** _bufptr1 = bufptrs1 + level_offset;
				Item** _bufptrEnd1 = bufptrsEnd1 + level_offset;
				// for large l2 buffers
				if ((i & 1) == 0) {	// level 2, 4 and so on
					FOR(j, nodes_at_level, 1) {
						//printf("[%llX, %llX], [%llX, %llX]\n", p1, p1 + l2_buf_n, p2, p2 + l2_buf_n);
						*_bufptr1 = p1; _bufptr1++;
						*_bufptrEnd1 = p1 + l2_buf_n; _bufptrEnd1++;
						p1 += l2_buf_n + (64 / sizeof(Item));
					}
				}
				else {
					FOR(j, nodes_at_level, 1) {
						//printf("[%llX, %llX], [%llX, %llX]\n", p1, p1 + buf_n, p2, p2 + buf_n);
						*_bufptr1 = p1; _bufptr1++;
						*_bufptrEnd1 = p1 + buf_n; _bufptrEnd1++;
						p1 += buf_n + (64 / sizeof(Item));
					}
				}
			}
			bufptrs1[0] = p1; bufptrsEnd1[0] = p1 + l2_buf_n;		// output buffer for the roots
			p1 += l2_buf_n + (64 / sizeof(Item));

			FOR_INIT(i, 1, this->LEAF_LEVEL, 1) {
				ui nodes_at_level = 1U << i;
				ui level_offset = i * HALF_WAY;
				Item** _bufptr2 = bufptrs2 + level_offset;
				Item** _bufptrEnd2 = bufptrsEnd2 + level_offset;
				// for large l2 buffers
				if ((i & 1) == 0) {	// level 2, 4 and so on
					FOR(j, nodes_at_level, 1) {
						//printf("[%llX, %llX], [%llX, %llX]\n", p1, p1 + l2_buf_n, p2, p2 + l2_buf_n);
						*_bufptr2 = p1; _bufptr2++;
						*_bufptrEnd2 = p1 + l2_buf_n; _bufptrEnd2++;
						p1 += l2_buf_n + (64 / sizeof(Item));	
					}
				}
				else {
					FOR(j, nodes_at_level, 1) { 
						//printf("[%llX, %llX], [%llX, %llX]\n", p1, p1 + buf_n, p2, p2 + buf_n);
						*_bufptr2 = p1; _bufptr2++;
						*_bufptrEnd2 = p1 + buf_n; _bufptrEnd2++;
						p1 += buf_n + (64 / sizeof(Item));	
					}
				}
			}
			bufptrs2[0] = p1; bufptrsEnd2[0] = p1 + l2_buf_n;

			this->nodes = nodes_left;
			this->IO = _IO;
		}

		FORCEINLINE void merge_cleanup() override {
			Merge4Way<Reg, Item, external>** nodes_left = this->nodes;
			FOR(i, this->NODES, 1) {
				delete nodes_left[i];
				delete nodes_right[i];
			}
			delete[] nodes_left;
			delete[] nodes_right;
		}

		// assuming A, C address origami_utils::aligned
		FORCEINLINE void merge(Item* A, Item* C, ui64 chunk, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) override {
			constexpr ui INC = sizeof(Reg) / sizeof(Item) * MTREE_NREG;
			this->prior_unalign_items = 0;
			this->post_unalign_items = 0;
			//printf("Merging: [%llX %llX] to [%llX %llX], Tot: %llu\n", A, A + chunk * WAY, C, C + chunk * WAY, chunk * WAY);
			// create local vars
			Merge4Way<Reg, Item, external>** nodes_left = this->nodes;
			Item** bufptrs1 = this->bufptrs, ** bufptrsEnd1 = this->bufptrsEnd;
			Merge4Way<Reg, Item, external>** nodes_right = this->nodes_right;
			Item** bufptrs2 = this->bufptrs2, ** bufptrsEnd2 = this->bufptrsEnd2;
			ui LEAF_LEVEL = this->LEAF_LEVEL;

			// initialization begin 
			// 1. init leaf pointers --> only leaf ptrs and output ptrs will change for each merge
			const ui HALF_WAY = WAY >> 1;
			ui leaf_level_offset = LEAF_LEVEL * HALF_WAY;
			Item** _bufptr1 = bufptrs1 + leaf_level_offset;
			Item** _bufptr2 = bufptrs2 + leaf_level_offset;
			Item** _bufptrEnd1 = bufptrsEnd1 + leaf_level_offset;
			Item** _bufptrEnd2 = bufptrsEnd2 + leaf_level_offset;
			FOR(i, HALF_WAY, 1) {
				Item* p = A + i * chunk;
				*_bufptr1 = p;
				*_bufptrEnd1 = p + chunk;
				_bufptr1++; _bufptrEnd1++;
			}

			FOR_INIT(i, HALF_WAY, WAY, 1) {
				Item* p = A + i * chunk;
				*_bufptr2 = p;
				*_bufptrEnd2 = p + chunk;
				_bufptr2++; _bufptrEnd2++;
			}

			// 2. init root ptr as output buffer
			Item* output = C, * outputEnd = C + WAY * chunk;

			ui idx_offset = 0; // NODES - 1;
			FOR(curr_level, LEAF_LEVEL, 2) {
				ui nodes_at_level = 1 << curr_level;
				FOR(i, nodes_at_level, 1) {
					ui node_idx = curr_level * HALF_WAY + i;
					ui lidx = (curr_level + 1) * HALF_WAY + (i << 1); ui lidx2 = (curr_level + 2) * HALF_WAY + (i << 2);
					nodes_left[idx_offset]->initialize(bufptrs1[lidx2], bufptrs1[lidx2 + 1], bufptrs1[lidx2 + 2], bufptrs1[lidx2 + 3],
						bufptrsEnd1[lidx2], bufptrsEnd1[lidx2 + 1], bufptrsEnd1[lidx2 + 2], bufptrsEnd1[lidx2 + 3],
						bufptrs1[lidx], bufptrs1[lidx + 1], bufptrsEnd1[lidx], bufptrsEnd1[lidx + 1],
						bufptrs1[node_idx], bufptrsEnd1[node_idx]);
					nodes_right[idx_offset]->initialize(bufptrs2[lidx2], bufptrs2[lidx2 + 1], bufptrs2[lidx2 + 2], bufptrs2[lidx2 + 3],
						bufptrsEnd2[lidx2], bufptrsEnd2[lidx2 + 1], bufptrsEnd2[lidx2 + 2], bufptrsEnd2[lidx2 + 3],
						bufptrs2[lidx], bufptrs2[lidx + 1], bufptrsEnd2[lidx], bufptrsEnd2[lidx + 1],
						bufptrs2[node_idx], bufptrsEnd2[node_idx]);
					++idx_offset;
				}
			}

			// check ptrs
			//nodes_left[0]->print_node();
			//nodes_right[0]->print_node();

			// Initialization
			if (this->LEVELS_4WAY == 1) {
				nodes_left[0]->merge_leaf_to_internal_init();
				nodes_left[0]->merge_leaf_to_internal();
				nodes_right[0]->merge_leaf_to_internal_init();
				nodes_right[0]->merge_leaf_to_internal();
			}
			else if (this->LEVELS_4WAY == 2) {
				FOR_INIT(i, 1, 5, 1) {
					nodes_left[i]->merge_leaf_to_internal_init();
					nodes_left[i]->merge_leaf_to_internal();
					nodes_right[i]->merge_leaf_to_internal_init();
					nodes_right[i]->merge_leaf_to_internal();
					//printf("Internal node contains: %llu; Exhaust: %lu\n", nodes[i]->outputEnd - nodes[i]->output, nodes[i]->exhaust);
				}
				// to handle the case when initilization empties out these nodes ** NOTE: need to add this to the following else condition
				nodes_left[0]->exhaust0 = nodes_left[1]->exhaust;	nodes_left[0]->leafEnd0 = nodes_left[1]->outputEnd;
				nodes_left[0]->exhaust1 = nodes_left[2]->exhaust;	nodes_left[0]->leafEnd1 = nodes_left[2]->outputEnd;
				nodes_left[0]->exhaust2 = nodes_left[3]->exhaust;	nodes_left[0]->leafEnd2 = nodes_left[3]->outputEnd;
				nodes_left[0]->exhaust3 = nodes_left[4]->exhaust;	nodes_left[0]->leafEnd3 = nodes_left[4]->outputEnd;

				nodes_right[0]->exhaust0 = nodes_right[1]->exhaust;	nodes_right[0]->leafEnd0 = nodes_right[1]->outputEnd;
				nodes_right[0]->exhaust1 = nodes_right[2]->exhaust;	nodes_right[0]->leafEnd1 = nodes_right[2]->outputEnd;
				nodes_right[0]->exhaust2 = nodes_right[3]->exhaust;	nodes_right[0]->leafEnd2 = nodes_right[3]->outputEnd;
				nodes_right[0]->exhaust3 = nodes_right[4]->exhaust;	nodes_right[0]->leafEnd3 = nodes_right[4]->outputEnd;

				nodes_left[0]->merge_internal_to_internal_init();
				nodes_left[0]->merge_internal_to_internal();
				nodes_right[0]->merge_internal_to_internal_init();
				nodes_right[0]->merge_internal_to_internal();
			}
			else {
				ui nodes_last_level = 1LU << ((this->LEVELS_4WAY - 1) << 1);
				ui idx = this->NODES - 1;
				FOR(i, nodes_last_level, 1) {
					nodes_left[idx]->merge_leaf_to_internal_init();
					nodes_left[idx]->merge_leaf_to_internal();
					nodes_right[idx]->merge_leaf_to_internal_init();
					nodes_right[idx]->merge_leaf_to_internal();

					if (idx > 0) {		// we have more than one node i.e. this node has some parent
						ui pidx = (idx - 1) >> 2;
						ui exhaust_left = nodes_left[idx]->exhaust;
						ui exhaust_right = nodes_right[idx]->exhaust;
						Item* _outputEnd_left = nodes_left[idx]->outputEnd;
						Item* _outputEnd_right = nodes_right[idx]->outputEnd;
						switch (idx & 3) {
						case 1:
							nodes_left[pidx]->exhaust0 = exhaust_left;
							nodes_right[pidx]->exhaust0 = exhaust_right;
							nodes_left[pidx]->leafEnd0 = _outputEnd_left;
							nodes_right[pidx]->leafEnd0 = _outputEnd_right;
							break;
						case 2:
							nodes_left[pidx]->exhaust1 = exhaust_left;
							nodes_right[pidx]->exhaust1 = exhaust_right;
							nodes_left[pidx]->leafEnd1 = _outputEnd_left;
							nodes_right[pidx]->leafEnd1 = _outputEnd_right;
							break;
						case 3:
							nodes_left[pidx]->exhaust2 = exhaust_left;
							nodes_right[pidx]->exhaust2 = exhaust_right;
							nodes_left[pidx]->leafEnd2 = _outputEnd_left;
							nodes_right[pidx]->leafEnd2 = _outputEnd_right;
							break;
						case 0:	// last node
							nodes_left[pidx]->exhaust3 = exhaust_left;
							nodes_right[pidx]->exhaust3 = exhaust_right;
							nodes_left[pidx]->leafEnd3 = _outputEnd_left;
							nodes_right[pidx]->leafEnd3 = _outputEnd_right;
							break;
						}
					}
					--idx;
				}
				for (int i = this->LEVELS_4WAY - 2; i >= 0; --i) {
					ui nodes_level = 1LU << (i << 1);
					FOR(j, nodes_level, 1) {
						nodes_left[idx]->merge_internal_to_internal_init();
						nodes_left[idx]->merge_internal_to_internal();
						nodes_right[idx]->merge_internal_to_internal_init();
						nodes_right[idx]->merge_internal_to_internal();
						if (idx > 0) {
							ui pidx = (idx - 1) >> 2;
							ui exhaust_left = nodes_left[idx]->exhaust;
							ui exhaust_right = nodes_right[idx]->exhaust;
							Item* _outputEnd_left = nodes_left[idx]->outputEnd;
							Item* _outputEnd_right = nodes_right[idx]->outputEnd;
							switch (idx & 3) {
							case 1:
								nodes_left[pidx]->exhaust0 = exhaust_left;
								nodes_right[pidx]->exhaust0 = exhaust_right;
								nodes_left[pidx]->leafEnd0 = _outputEnd_left;
								nodes_right[pidx]->leafEnd0 = _outputEnd_right;
								break;
							case 2:
								nodes_left[pidx]->exhaust1 = exhaust_left;
								nodes_right[pidx]->exhaust1 = exhaust_right;
								nodes_left[pidx]->leafEnd1 = _outputEnd_left;
								nodes_right[pidx]->leafEnd1 = _outputEnd_right;
								break;
							case 3:
								nodes_left[pidx]->exhaust2 = exhaust_left;
								nodes_right[pidx]->exhaust2 = exhaust_right;
								nodes_left[pidx]->leafEnd2 = _outputEnd_left;
								nodes_right[pidx]->leafEnd2 = _outputEnd_right;
								break;
							case 0:	// last node
								nodes_left[pidx]->exhaust3 = exhaust_left;
								nodes_right[pidx]->exhaust3 = exhaust_right;
								nodes_left[pidx]->leafEnd3 = _outputEnd_left;
								nodes_right[pidx]->leafEnd3 = _outputEnd_right;
								break;
							}
						}
						--idx;
					}
				}
			}

			//nodes[0]->merge_internal_to_root_init();

			Merge4Way<Reg, Item, external>* root_left = nodes_left[0];
			Merge4Way<Reg, Item, external>* root_right = nodes_right[0];
			Item* loadFrom = root_left->output;
			Item* opposite = root_right->output;
			Item* endA = root_left->outputEnd;
			Item* endB = root_right->outputEnd;

#if MTREE_NREG == 1
			Reg a1;
			origami_utils::load<Reg>(a1, (Reg*)opposite); opposite += INC;

			while (1) {
				Item* o = output;
				merge_root_aligned(&loadFrom, &opposite, a1, &output, endA, endB, root_left->exhaust, root_right->exhaust);
				/*while (o < output) printf("%lu ", *o++);
				printf("\n");*/
				if (output >= outputEnd) break;
				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;


			}

#elif NREG == 2
			Reg a2, a3;
			origami_utils::load<Reg>(a2, (Reg*)opposite); 
			origami_utils::load<Reg>(a3, (Reg*)opposite + 1); opposite += INC;

			while (1) {
				Item* o = output;
				merge_root_aligned(&loadFrom, &opposite, a2, a3, &output, endA, endB, root_left->exhaust, root_right->exhaust);
				/*while (o < output) printf("%lu ", *o++);
				printf("\n");*/
				if (output >= outputEnd) break;
				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;


			}
#endif 

			/*
			// Merge
			if (LEVELS_4WAY == 1) {
				nodes[0]->merge_leaf_to_root();
				return;
			}
			ui empty = 0;
			while (nodes[0]->output < outputEnd) {
				Item* o = nodes[0]->output;
				empty = nodes[0]->merge_internal_to_root();
				Item* oNew = nodes[0]->output;
				//PRINT_ARR(o, oNew - o);

				Item* loadFrom0 = nodes[0]->loadFrom0;
				Item* loadFrom1 = nodes[0]->loadFrom1;
				Item* opposite0 = nodes[0]->opposite0;
				Item* opposite1 = nodes[0]->opposite1;

				ui i = 0;
				while (i < 2) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode<Reg, Item>(nodes, cidx, 1, LEVELS_4WAY);
						loadFrom0 = nodes[cidx]->output;

						bool first = *loadFrom0 < *opposite0;
						Item* tmp0 = first ? loadFrom0 : opposite0;
						opposite0 = first ? opposite0 : loadFrom0;
						loadFrom0 = tmp0;
					}
					++i;
				}
				while (i < 4) {
					ui cidx = i + 1;
					if (((empty >> i) & 1) && (nodes[cidx]->exhaust < 2)) {
						RefillNode<Reg, Item>(nodes, cidx, 1, LEVELS_4WAY);
						loadFrom1 = nodes[cidx]->output;

						bool first = *loadFrom1 < *opposite1;
						Item* tmp0 = first ? loadFrom1 : opposite1;
						opposite1 = first ? opposite1 : loadFrom1;
						loadFrom1 = tmp0;
					}
					++i;
				}

				nodes[0]->exhaust0 = nodes[1]->exhaust;	nodes[0]->leafEnd0 = nodes[1]->outputEnd;
				nodes[0]->exhaust1 = nodes[2]->exhaust;	nodes[0]->leafEnd1 = nodes[2]->outputEnd;
				nodes[0]->exhaust2 = nodes[3]->exhaust;	nodes[0]->leafEnd2 = nodes[3]->outputEnd;
				nodes[0]->exhaust3 = nodes[4]->exhaust;	nodes[0]->leafEnd3 = nodes[4]->outputEnd;

				nodes[0]->loadFrom0 = loadFrom0;
				nodes[0]->loadFrom1 = loadFrom1;
				nodes[0]->opposite0 = opposite0;
				nodes[0]->opposite1 = opposite1;
			}
			*/
		}

		FORCEINLINE void merge(Item** _X, Item** _endX, Item* C, ui64 n, ui buf_n, ui l2_buf_n, Item* buf, ui WAY) override {
			// debug: check if lists sorted
			/*FOR(i, WAY, 1) {
				printf("Checking stream %u w/ %llu items ... ", i, (_endX[i] - _X[i]));
				SortCorrectnessChecker<Item>(_X[i], (_endX[i] - _X[i]));
				printf("done\n");
			}
			return;*/


			constexpr ui INC = sizeof(Reg) / sizeof(Item) * MTREE_NREG;
			this->prior_unalign_items = 0;
			this->post_unalign_items = 0;
			//printf("Merging: [%llX %llX] to [%llX %llX], Tot: %llu\n", A, A + chunk * WAY, C, C + chunk * WAY, chunk * WAY);
				// create local vars
			Merge4Way<Reg, Item, external>** nodes_left = this->nodes;
			Item** bufptrs1 = this->bufptrs, ** bufptrsEnd1 = this->bufptrsEnd;
			Merge4Way<Reg, Item, external>** nodes_right = this->nodes_right;
			Item** bufptrs2 = this->bufptrs2, ** bufptrsEnd2 = this->bufptrsEnd2;
			ui LEAF_LEVEL = this->LEAF_LEVEL;

			ui64 n_temp = 0;
			C = (Item*)(origami_utils::align<Reg, MTREE_NREG>((char*)C));

			// initialization begin 
			// 1. init leaf pointers
			const ui HALF_WAY = WAY >> 1;
			ui leaf_level_offset = LEAF_LEVEL * HALF_WAY;
			Item** _bufptr1 = bufptrs1 + leaf_level_offset;
			Item** _bufptr2 = bufptrs2 + leaf_level_offset;
			Item** _bufptrEnd1 = bufptrsEnd1 + leaf_level_offset;
			Item** _bufptrEnd2 = bufptrsEnd2 + leaf_level_offset;
			FOR(i, HALF_WAY, 1) {
				Item* x = _X[i];
				Item* aligned_x = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)x);
				Item* endx = _endX[i];
				bool endx_aligned = origami_utils::aligned<Reg, MTREE_NREG>((char*)endx);
				Item* endx2 = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)endx) + INC;
				Item* aligned_endx = endx_aligned ? endx : endx2;

				this->prior_unalign_items += x - aligned_x;
				this->post_unalign_items += aligned_endx - endx;

				*_bufptr1 = aligned_x;
				*_bufptrEnd1 = aligned_endx;
				n_temp += (endx - x);
				_bufptr1++; _bufptrEnd1++;

				//printf("[%llX, %llX] -> [%llX, %llX]\n", _X[i], _endX[i], aligned_x, aligned_endx);
			}
			FOR_INIT(i, HALF_WAY, WAY, 1) {
				/*Item* x = _X[i];
				Item* endx = (Item*)(origami_utils::align<Reg>(_endX[i]));
				bool origami_utils::aligned = origami_utils::aligned<Reg>(x);
				Item* y = (Item*)origami_utils::align<Reg>(x) + INC;
				x = origami_utils::aligned ? x : y;
				*_bufptr2 = x;
				*_bufptrEnd2 = endx;
				n_temp += (endx - x);
				_bufptr2++; _bufptrEnd2++;*/

				Item* x = _X[i];
				Item* aligned_x = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)x);
				Item* endx = _endX[i];
				bool endx_aligned = origami_utils::aligned<Reg, MTREE_NREG>((char*)endx);
				Item* endx2 = (Item*)origami_utils::align<Reg, MTREE_NREG>((char*)endx) + INC;
				Item* aligned_endx = endx_aligned ? endx : endx2;

				//printf("Prior unalign: %lu, Post unalign: %lu\n", x - aligned_x, aligned_endx - endx);
				this->prior_unalign_items += x - aligned_x;
				this->post_unalign_items += aligned_endx - endx;

				*_bufptr2 = aligned_x;
				*_bufptrEnd2 = aligned_endx;
				n_temp += (endx - x);
				_bufptr2++; _bufptrEnd2++;
			}

			//printf("Prior unalign: %lu\n", prior_unalign_items);

			// 2. init root ptr as output buffer
			Item* output = C, * outputEnd = C + n;

			ui idx_offset = 0; // NODES - 1;
			FOR(curr_level, LEAF_LEVEL, 2) {
				ui nodes_at_level = 1 << curr_level;
				FOR(i, nodes_at_level, 1) {
					ui node_idx = curr_level * HALF_WAY + i;
					ui lidx = (curr_level + 1) * HALF_WAY + (i << 1); ui lidx2 = (curr_level + 2) * HALF_WAY + (i << 2);
					nodes_left[idx_offset]->initialize(bufptrs1[lidx2], bufptrs1[lidx2 + 1], bufptrs1[lidx2 + 2], bufptrs1[lidx2 + 3],
						bufptrsEnd1[lidx2], bufptrsEnd1[lidx2 + 1], bufptrsEnd1[lidx2 + 2], bufptrsEnd1[lidx2 + 3],
						bufptrs1[lidx], bufptrs1[lidx + 1], bufptrsEnd1[lidx], bufptrsEnd1[lidx + 1],
						bufptrs1[node_idx], bufptrsEnd1[node_idx]);
					nodes_right[idx_offset]->initialize(bufptrs2[lidx2], bufptrs2[lidx2 + 1], bufptrs2[lidx2 + 2], bufptrs2[lidx2 + 3],
						bufptrsEnd2[lidx2], bufptrsEnd2[lidx2 + 1], bufptrsEnd2[lidx2 + 2], bufptrsEnd2[lidx2 + 3],
						bufptrs2[lidx], bufptrs2[lidx + 1], bufptrsEnd2[lidx], bufptrsEnd2[lidx + 1],
						bufptrs2[node_idx], bufptrsEnd2[node_idx]);
					++idx_offset;
				}
			}

			// Initialization
			if (this->LEVELS_4WAY == 1) {
				nodes_left[0]->merge_leaf_to_internal_init(0, this->IO);
				nodes_left[0]->merge_leaf_to_internal(0, this->IO);
				nodes_right[0]->merge_leaf_to_internal_init(4, this->IO);
				nodes_right[0]->merge_leaf_to_internal(4, this->IO);
			}
			else if (this->LEVELS_4WAY == 2) {
				FOR_INIT(i, 1, 5, 1) {
					nodes_left[i]->merge_leaf_to_internal_init((i - 1) << 2, this->IO);
					nodes_left[i]->merge_leaf_to_internal((i - 1) << 2, this->IO);
					nodes_right[i]->merge_leaf_to_internal_init(HALF_WAY + ((i - 1) << 2), this->IO);
					nodes_right[i]->merge_leaf_to_internal(HALF_WAY + ((i - 1) << 2), this->IO);
					//printf("Internal node contains: %llu; Exhaust: %lu\n", nodes[i]->outputEnd - nodes[i]->output, nodes[i]->exhaust);
				}
				// to handle the case when initilization empties out these nodes ** NOTE: need to add this to the following else condition
				nodes_left[0]->exhaust0 = nodes_left[1]->exhaust;	nodes_left[0]->leafEnd0 = nodes_left[1]->outputEnd;
				nodes_left[0]->exhaust1 = nodes_left[2]->exhaust;	nodes_left[0]->leafEnd1 = nodes_left[2]->outputEnd;
				nodes_left[0]->exhaust2 = nodes_left[3]->exhaust;	nodes_left[0]->leafEnd2 = nodes_left[3]->outputEnd;
				nodes_left[0]->exhaust3 = nodes_left[4]->exhaust;	nodes_left[0]->leafEnd3 = nodes_left[4]->outputEnd;

				nodes_right[0]->exhaust0 = nodes_right[1]->exhaust;	nodes_right[0]->leafEnd0 = nodes_right[1]->outputEnd;
				nodes_right[0]->exhaust1 = nodes_right[2]->exhaust;	nodes_right[0]->leafEnd1 = nodes_right[2]->outputEnd;
				nodes_right[0]->exhaust2 = nodes_right[3]->exhaust;	nodes_right[0]->leafEnd2 = nodes_right[3]->outputEnd;
				nodes_right[0]->exhaust3 = nodes_right[4]->exhaust;	nodes_right[0]->leafEnd3 = nodes_right[4]->outputEnd;

				nodes_left[0]->merge_internal_to_internal_init();
				nodes_left[0]->merge_internal_to_internal();
				nodes_right[0]->merge_internal_to_internal_init();
				nodes_right[0]->merge_internal_to_internal();
			}
			else {
				ui nodes_last_level = 1LU << ((this->LEVELS_4WAY - 1) << 1);
				ui idx = this->NODES - 1;
				ui leaf_base = this->NODES - (HALF_WAY >> 2);
				FOR(i, nodes_last_level, 1) {
					ui buf_idx =(idx - leaf_base) << 2;
					nodes_left[idx]->merge_leaf_to_internal_init(buf_idx, this->IO);
					nodes_left[idx]->merge_leaf_to_internal(buf_idx, this->IO);
					nodes_right[idx]->merge_leaf_to_internal_init(HALF_WAY + buf_idx, this->IO);
					nodes_right[idx]->merge_leaf_to_internal(HALF_WAY + buf_idx, this->IO);

					if (idx > 0) {		// we have more than one node i.e. this node has some parent
						ui pidx = (idx - 1) >> 2;
						ui exhaust_left = nodes_left[idx]->exhaust;
						ui exhaust_right = nodes_right[idx]->exhaust;
						Item* _outputEnd_left = nodes_left[idx]->outputEnd;
						Item* _outputEnd_right = nodes_right[idx]->outputEnd;
						switch (idx & 3) {
						case 1:
							nodes_left[pidx]->exhaust0 = exhaust_left;
							nodes_right[pidx]->exhaust0 = exhaust_right;
							nodes_left[pidx]->leafEnd0 = _outputEnd_left;
							nodes_right[pidx]->leafEnd0 = _outputEnd_right;
							break;
						case 2:
							nodes_left[pidx]->exhaust1 = exhaust_left;
							nodes_right[pidx]->exhaust1 = exhaust_right;
							nodes_left[pidx]->leafEnd1 = _outputEnd_left;
							nodes_right[pidx]->leafEnd1 = _outputEnd_right;
							break;
						case 3:
							nodes_left[pidx]->exhaust2 = exhaust_left;
							nodes_right[pidx]->exhaust2 = exhaust_right;
							nodes_left[pidx]->leafEnd2 = _outputEnd_left;
							nodes_right[pidx]->leafEnd2 = _outputEnd_right;
							break;
						case 0:	// last node
							nodes_left[pidx]->exhaust3 = exhaust_left;
							nodes_right[pidx]->exhaust3 = exhaust_right;
							nodes_left[pidx]->leafEnd3 = _outputEnd_left;
							nodes_right[pidx]->leafEnd3 = _outputEnd_right;
							break;
						}
					}
					--idx;
				}
				for (int i = this->LEVELS_4WAY - 2; i >= 0; --i) {
					ui nodes_level = 1LU << (i << 1);
					FOR(j, nodes_level, 1) {
						nodes_left[idx]->merge_internal_to_internal_init();
						nodes_left[idx]->merge_internal_to_internal();
						nodes_right[idx]->merge_internal_to_internal_init();
						nodes_right[idx]->merge_internal_to_internal();
						if (idx > 0) {
							ui pidx = (idx - 1) >> 2;
							ui exhaust_left = nodes_left[idx]->exhaust;
							ui exhaust_right = nodes_right[idx]->exhaust;
							Item* _outputEnd_left = nodes_left[idx]->outputEnd;
							Item* _outputEnd_right = nodes_right[idx]->outputEnd;
							switch (idx & 3) {
							case 1:
								nodes_left[pidx]->exhaust0 = exhaust_left;
								nodes_right[pidx]->exhaust0 = exhaust_right;
								nodes_left[pidx]->leafEnd0 = _outputEnd_left;
								nodes_right[pidx]->leafEnd0 = _outputEnd_right;
								break;
							case 2:
								nodes_left[pidx]->exhaust1 = exhaust_left;
								nodes_right[pidx]->exhaust1 = exhaust_right;
								nodes_left[pidx]->leafEnd1 = _outputEnd_left;
								nodes_right[pidx]->leafEnd1 = _outputEnd_right;
								break;
							case 3:
								nodes_left[pidx]->exhaust2 = exhaust_left;
								nodes_right[pidx]->exhaust2 = exhaust_right;
								nodes_left[pidx]->leafEnd2 = _outputEnd_left;
								nodes_right[pidx]->leafEnd2 = _outputEnd_right;
								break;
							case 0:	// last node
								nodes_left[pidx]->exhaust3 = exhaust_left;
								nodes_right[pidx]->exhaust3 = exhaust_right;
								nodes_left[pidx]->leafEnd3 = _outputEnd_left;
								nodes_right[pidx]->leafEnd3 = _outputEnd_right;
								break;
							}
						}
						--idx;
					}
				}
			}


			Merge4Way<Reg, Item, external>* root_left = nodes_left[0];
			Merge4Way<Reg, Item, external>* root_right = nodes_right[0];
			Item* loadFrom = root_left->output;
			Item* opposite = root_right->output;
			Item* endA = root_left->outputEnd;
			Item* endB = root_right->outputEnd;
#if MTREE_NREG == 1
			Reg a1;
			origami_utils::load<Reg>(a1, (Reg*)opposite); opposite += INC;

			// skip prior_unalign_items keys
			while (1) {
				merge_root_unaligned_skip_prior(&loadFrom, &opposite, a1, endA, endB, root_left->exhaust, root_right->exhaust, &this->prior_unalign_items);
				if (this->prior_unalign_items == 0) break;

				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY, this->NODES - (HALF_WAY >> 2), 0, this->IO);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY, this->NODES - (HALF_WAY >> 2), HALF_WAY, this->IO);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;
			}

			while (1) {
				if constexpr (external) merge_root_unaligned(&loadFrom, &opposite, a1, &output, endA, endB, root_left->exhaust, root_right->exhaust, outputEnd, this->IO);
				else merge_root_unaligned(&loadFrom, &opposite, a1, &output, endA, endB, root_left->exhaust, root_right->exhaust, outputEnd);

				if (output >= outputEnd) break;
				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY, this->NODES - (HALF_WAY >> 2), 0, this->IO);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY, this->NODES - (HALF_WAY >> 2), HALF_WAY, this->IO);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;
			}
#elif NREG == 2
			Reg a2, a3;
			origami_utils::load<Reg>(a2, (Reg*)opposite);
			origami_utils::load<Reg>(a3, (Reg*)opposite + 1); opposite += INC;

			// skip prior_unalign_items keys
			while (1) {
				merge_root_unaligned_skip_prior(&loadFrom, &opposite, a2, a3, endA, endB, root_left->exhaust, root_right->exhaust, &this->prior_unalign_items);
				if (this->prior_unalign_items == 0) break;

				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;
			}

			while (1) {
				merge_root_unaligned(&loadFrom, &opposite, a2, a3, &output, endA, endB, root_left->exhaust, root_right->exhaust, outputEnd);

				if (output >= outputEnd) break;
				if (loadFrom == endA && root_left->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_left, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_left->output;
					endA = root_left->outputEnd;
				}
				else if (loadFrom == endB && root_right->exhaust < 2) {
					RefillNode<Reg, Item>(nodes_right, 0, 0, this->LEVELS_4WAY);
					loadFrom = root_right->output;
					endB = root_right->outputEnd;
				}
				bool first = *loadFrom < *opposite;
				Item* tmp = first ? loadFrom : opposite;
				opposite = first ? opposite : loadFrom;
				loadFrom = tmp;
			}
#endif
			
		}
	};
}

