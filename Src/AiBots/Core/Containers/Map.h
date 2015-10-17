// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"

#include "Core/Containers/Vector.h"

// #define RBTREE_VERIFY

namespace Rio
{

	// Map from key to value. Uses Vector internally
	// Non-POD items
	// Calls ctor/dtor
	// not for performance-critical stuff
	// Internally Map is Red-Black tree
	template <typename TKey, typename TValue>
	struct Map
	{
		Map(Allocator& a);
		const TValue& operator[](const TKey& key) const;
		struct Node
		{
			TKey key;
			TValue value;
			size_t left;
			size_t right;
			size_t parent;
			uint32_t color;
		};

		size_t root;
		// special node traversal path terminator
		size_t sentinel;
		Vector<Node> data;
	};

	// Functions to manipulate Map
	namespace MapFn
	{
		template <typename TKey, typename TValue> size_t getCount(const Map<TKey, TValue>& m);
		// Returns whether the given key exists in the map
		template <typename TKey, typename TValue> bool has(const Map<TKey, TValue>& m, const TKey& key);
		// Returns the value for the given key or default if the key does not exist in the map.
		template <typename TKey, typename TValue> const TValue& get(const Map<TKey, TValue>& m, const TKey& key, const TValue& defaultValue);
		// Sets the value for the key in the map.
		template <typename TKey, typename TValue> void set(Map<TKey, TValue>& m, const TKey& key, const TValue& value);
		// Removes the key from the map if it exists.
		template <typename TKey, typename TValue> void remove(Map<TKey, TValue>& m, const TKey& key);
		// Removes all the items in the map.
		// Calls destructor on the items.
		template <typename TKey, typename TValue> void clear(Map<TKey, TValue>& m);
		// Returns a pointer to the first item in the map
		// Can be used to efficiently iterate over the elements (in random order)
		template <typename TKey, typename TValue> const typename Map<TKey, TValue>::Node* begin(const Map<TKey, TValue>& m);
		template <typename TKey, typename TValue> const typename Map<TKey, TValue>::Node* end(const Map<TKey, TValue>& m);
	} // namespace MapFn

	namespace MapInternal
	{
		const uint32_t BLACK = 0xB1B1B1B1u;
		const uint32_t RED = 0xEDEDEDEDu;
		const size_t NIL = 0xFFFFFFFFFFFFFFFFu;

		template <typename TKey, typename TValue>
		inline size_t getRoot(const Map<TKey, TValue>& m)
		{
			return m.root;
		}

		template <typename TKey, typename TValue>
		inline size_t getParent(const Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);
			return m.data[n].parent;
		}

		template <typename TKey, typename TValue>
		inline size_t getLeft(const Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);
			return m.data[n].left;
		}

		template <typename TKey, typename TValue>
		inline size_t getRight(const Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);
			return m.data[n].right;
		}

		template <typename TKey, typename TValue>
		inline uint32_t getColor(const Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);
			return m.data[n].color;
		}

#ifdef RBTREE_VERIFY
		template<typename TKey, typename TValue>
		inline int32_t debugVerify(Map<TKey, TValue>& m, size_t n)
		{
			if (n == m.sentinel)
			{
				return 0;
			}

			if (getLeft(m, n) != m.sentinel)
			{
				RIO_ASSERT(getParent(m, getLeft(m, n)) == n, "Bad RBTree");
				RIO_ASSERT(m.data[getLeft(m, n)].key < m.data[n].key, "Bad RBTree");
			}

			if (getRight(m, n) != m.sentinel)
			{
				RIO_ASSERT(getParent(m, getRight(m, n)) == n, "Bad RBTree");
				RIO_ASSERT(m.data[n].key < m.data[getRight(m, n)].key, "Bad RBTree");
			}

			int32_t bhLeft = debugVerify(m, getLeft(m, n));
			int32_t bhRight = debugVerify(m, getRight(m, n));
			RIO_ASSERT(bhLeft == bhRight, "Bad RBTree");

			if (getColor(m, n) == MapInternal::BLACK)
			{
				bhLeft += 1;
			}
			else
			{
				if (getParent(m, n) != MapInternal::NIL && getColor(m, getParent(m, n)) == MapInternal::RED)
				{
					RIO_ASSERT(false, "Bad RBTree");
				}
			}

			return bhLeft;
		}

		template<typename TKey, typename TValue>
		inline int32_t dump(Map<TKey, TValue>& m)
		{
			for (size_t i = 0; i < VectorFn::getCount(m.data); i++)
			{
				printf("%d = [%d, %d, %d] ", i, getParent(m, i), getLeft(m, i), getRight(m, i));
			}
			printf("\n");
			return 0;
		}
#endif

		template <typename TKey, typename TValue>
		inline size_t getMin(const Map<TKey, TValue>& m, size_t n)
		{
			if (n == m.sentinel)
			{
				return n;
			}
			while (getLeft(m, n) != m.sentinel)
			{
				n = getLeft(m, n);
			}
			return n;
		}

		template <typename TKey, typename TValue>
		inline size_t getMax(const Map<TKey, TValue>& m, size_t x)
		{
			if (x == m.sentinel)
			{
				return x;
			}
			while (getRight(m, x) != m.sentinel)
			{
				x = getRight(m, x);
			}
			return x;
		}

		template <typename TKey, typename TValue>
		inline size_t getSuccessor(const Map<TKey, TValue>& m, size_t x)
		{
			if (getRight(m, x) != m.sentinel)
			{
				return getMin(m, getRight(m, x));
			}
			size_t y = getParent(m, x);
			while (y != MapInternal::NIL && x == getRight(m, y))
			{
				x = y;
				y = getParent(m, y);
			}
			return y;
		}

		template <typename TKey, typename TValue>
		inline size_t getPredecessor(const Map<TKey, TValue>& m, size_t x)
		{
			if (getLeft(m, x) != m.sentinel)
			{
				return getMax(m, getLeft(m, x));
			}
			size_t y = getParent(m, x);
			while (y != MapInternal::NIL && x == getLeft(m, y))
			{
				x = y;
				y = getParent(m, y);
			}
			return y;
		}

		template <typename TKey, typename TValue>
		inline void rotateLeft(Map<TKey, TValue>& m, size_t x)
		{
			RIO_ASSERT(x < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), x);

			size_t y = getRight(m, x);
			m.data[x].right = getLeft(m, y);

			if (getLeft(m, y) != m.sentinel)
			{
				m.data[getLeft(m, y)].parent = x;
			}

			m.data[y].parent = getParent(m, x);

			if (getParent(m, x) == MapInternal::NIL)
			{
				m.root = y;
			}
			else
			{
				if (x == getLeft(m, getParent(m, x)))
				{
					m.data[getParent(m, x)].left = y;
				}
				else
				{
					m.data[getParent(m, x)].right = y;
				}
			}

			m.data[y].left = x;
			m.data[x].parent = y;
		}

		template <typename TKey, typename TValue>
		inline void rotateRight(Map<TKey, TValue>& m, size_t x)
		{
			RIO_ASSERT(x < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), x);

			uint32_t y = getLeft(m, x);
			m.data[x].left = getRight(m, y);

			if (getRight(m, y) != m.sentinel)
			{
				m.data[getRight(m, y)].parent = x;
			}

			m.data[y].parent = getParent(m, x);

			if (getParent(m, x) == MapInternal::NIL)
			{
				m.root = y;
			}
			else
			{
				if (x == getLeft(m, getParent(m, x)))
				{
					m.data[getParent(m, x)].left = y;
				}
				else
				{
					m.data[getParent(m, x)].right = y;
				}
			}
			m.data[y].right = x;
			m.data[x].parent = y;
		}

		template <typename TKey, typename TValue>
		inline void destroy(Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);
			size_t x = VectorFn::getCount(m.data) - 1;
			if (x == m.root)
			{
				m.root = n;

				if (getLeft(m, x) != MapInternal::NIL)
				{
					m.data[getLeft(m, x)].parent = n;
				}
				if (getRight(m, x) != MapInternal::NIL)
				{
					m.data[getRight(m, x)].parent = n;
				}
				m.data[n] = m.data[x];
			}
			else
			{
				if (x != n)
				{
					if (x == getLeft(m, getParent(m, x)))
					{
						m.data[getParent(m, x)].left = n;
					}
					else if (x == getRight(m, getParent(m, x)))
					{
						m.data[getParent(m, x)].right = n;
					}
					if (getLeft(m, x) != MapInternal::NIL)
					{
						m.data[getLeft(m, x)].parent = n;
					}
					if (getRight(m, x) != MapInternal::NIL)
					{
						m.data[getRight(m, x)].parent = n;
					}
					m.data[n] = m.data[x];
				}
			}

#ifdef RBTREE_VERIFY
			debugVerify(m, m.root);
#endif // RBTREE_VERIFY
			VectorFn::popBack(m.data);
		}

		template <typename TKey, typename TValue>
		inline void insertFixup(Map<TKey, TValue>& m, size_t n)
		{
			RIO_ASSERT(n < VectorFn::getCount(m.data), "Index out of bounds (size = %d, n = %d)", VectorFn::getCount(m.data), n);

			size_t x;
			size_t y;

			while (n != getRoot(m) && getColor(m, getParent(m, n)) == MapInternal::RED)
			{
				x = getParent(m, n);

				if (x == getLeft(m, getParent(m, x)))
				{
					y = getRight(m, getParent(m, x));

					if (getColor(m, y) == MapInternal::RED)
					{
						m.data[x].color = MapInternal::BLACK;
						m.data[y].color = MapInternal::BLACK;
						m.data[getParent(m, x)].color = MapInternal::RED;
						n = getParent(m, x);
						continue;
					}
					else
					{
						if (n == getRight(m, x))
						{
							n = x;
							rotateLeft(m, n);
							x = getParent(m, n);
						}

						m.data[x].color = MapInternal::BLACK;
						m.data[getParent(m, x)].color = MapInternal::RED;
						rotateRight(m, getParent(m, x));
					}
				}
				else
				{
					y = getLeft(m, getParent(m, x));

					if (getColor(m, y) == MapInternal::RED)
					{
						m.data[x].color = MapInternal::BLACK;
						m.data[y].color = MapInternal::BLACK;
						m.data[getParent(m, x)].color = MapInternal::RED;
						n = getParent(m, x);
						continue;
					}
					else
					{
						if (n == getLeft(m, x))
						{
							n = x;
							rotateRight(m, n);
							x = getParent(m, n);
						}

						m.data[x].color = MapInternal::BLACK;
						m.data[getParent(m, x)].color = MapInternal::RED;
						rotateLeft(m, getParent(m, x));
					}
				}
			}
		}

		template <typename TKey, typename TValue>
		inline size_t innerFind(const Map<TKey, TValue>& m, const TKey& key)
		{
			size_t x = m.root;

			while (x != m.sentinel)
			{
				if (m.data[x].key < key)
				{
					if (getRight(m, x) == m.sentinel)
					{
						return x;
					}

					x = getRight(m, x);
				}
				else if (key < m.data[x].key)
				{
					if (getLeft(m, x) == m.sentinel)
					{
						return x;
					}

					x = getLeft(m, x);
				}
				else
				{
					break;
				}
			}

			return x;
		}

		template <typename TKey, typename TValue>
		inline size_t findOrFail(const Map<TKey, TValue>& m, const TKey& key)
		{
			size_t p = innerFind(m, key);
			if (p != m.sentinel && m.data[p].key == key)
			{
				return p;
			}
			return MapInternal::NIL;
		}

		template <typename TKey, typename TValue>
		inline size_t findOrAdd(Map<TKey, TValue>& m, const TKey& key)
		{
			size_t p = innerFind(m, key);

			if (p != m.sentinel && m.data[p].key == key)
			{
				return p;
			}

			typename Map<TKey, TValue>::Node n;
			n.key = key;
			n.value = TValue();
			n.color = MapInternal::RED;
			n.left = m.sentinel;
			n.right = m.sentinel;
			n.parent = MapInternal::NIL;

			if (p == m.sentinel)
			{
				m.root = n;
			}
			else
			{
				if (key < m.data[p].key)
				{
					m.data[p].left = n;
				}
				else
				{
					m.data[p].right = n;
				}

				m.data[n].parent = p;
			}

			add_fixup(m, n);
			m.data[m.root].color = MapInternal::BLACK;
#ifdef RBTREE_VERIFY
			debugVerify(m, m.root);
#endif
			return n;
		}
	} // namespace MapInternal

	namespace MapFn
	{
		template <typename TKey, typename TValue>
		size_t getCount(const Map<TKey, TValue>& m)
		{
			RIO_ASSERT(VectorFn::getCount(m.data) > 0, "Bad Map"); // There should be at least sentinel
			return VectorFn::getCount(m.data) - 1;
		}

		template <typename TKey, typename TValue>
		inline bool has(const Map<TKey, TValue>& m, const TKey& key)
		{
			return MapInternal::findOrFail(m, key) != MapInternal::NIL;
		}

		template <typename TKey, typename TValue>
		inline const TValue& get(const Map<TKey, TValue>& m, const TKey& key, const TValue& defaultValue)
		{
			size_t p = MapInternal::innerFind(m, key);

			if (p != m.sentinel && m.data[p].key == key)
			{
				return m.data[p].value;
			}

			return defaultValue;
		}

		template <typename TKey, typename TValue>
		inline void set(Map<TKey, TValue>& m, const TKey& key, const TValue& value)
		{
			typename Map<TKey, TValue>::Node node;
			node.key = key;
			node.value = value;
			node.color = MapInternal::RED;
			node.left = m.sentinel;
			node.right = m.sentinel;
			node.parent = MapInternal::NIL;
			size_t n = VectorFn::pushBack(m.data, node);
			size_t x = m.root;
			size_t y = MapInternal::NIL;

			if (x == m.sentinel)
			{
				m.root = n;
			}
			else
			{
				while (x != m.sentinel)
				{
					y = x;

					if (key < m.data[x].key)
						x = m.data[x].left;
					else
						x = m.data[x].right;
				}

				if (key < m.data[y].key)
					m.data[y].left = n;
				else
					m.data[y].right = n;

				m.data[n].parent = y;
			}

			MapInternal::insertFixup(m, n);
			m.data[m.root].color = MapInternal::BLACK;
#ifdef RBTREE_VERIFY
			MapInternal::debugVerify(m, m.root);
#endif
		}

		template <typename TKey, typename TValue>
		inline void remove(Map<TKey, TValue>& m, const TKey& key)
		{
			using namespace MapInternal;

			size_t n = innerFind(m, key);

			if (!(m.data[n].key == key))
			{
				return;
			}

			size_t x;
			size_t y;

			if (getLeft(m, n) == m.sentinel || getRight(m, n) == m.sentinel)
			{
				y = n;
			}
			else
			{
				y = getSuccessor(m, n);
			}

			if (getLeft(m, y) != m.sentinel)
			{
				x = getLeft(m, y);
			}
			else
			{
				x = getRight(m, y);
			}

			m.data[x].parent = getParent(m, y);

			if (getParent(m, y) != MapInternal::NIL)
			{
				if (y == getLeft(m, getParent(m, y)))
				{
					m.data[getParent(m, y)].left = x;
				}
				else
				{
					m.data[getParent(m, y)].right = x;
				}
			}
			else
			{
				m.root = x;
			}

			if (y != n)
			{
				m.data[n].key = m.data[y].key;
				m.data[n].value = m.data[y].value;
			}

			// Do the fixup
			if (getColor(m, y) == MapInternal::BLACK)
			{
				size_t y;
				while (x != m.root && getColor(m, x) == MapInternal::BLACK)
				{
					if (x == getLeft(m, getParent(m, x)))
					{
						y = getRight(m, getParent(m, x));

						if (getColor(m, y) == MapInternal::RED)
						{
							m.data[y].color = MapInternal::BLACK;
							m.data[getParent(m, x)].color = MapInternal::RED;
							rotateLeft(m, getParent(m, x));
							y = getRight(m, getParent(m, x));
						}

						if (getColor(m, getLeft(m, y)) == MapInternal::BLACK && color(m, getRight(m, y)) == MapInternal::BLACK)
						{
							m.data[y].color = MapInternal::RED;
							x = getParent(m, x);
						}
						else
						{
							if (getColor(m, getRight(m, y)) == MapInternal::BLACK)
							{
								m.data[getLeft(m, y)].color = MapInternal::BLACK;
								m.data[y].color = MapInternal::RED;
								rotateRight(m, y);
								y = getRight(m, getParent(m, x));
							}

							m.data[y].color = getColor(m, getParent(m, x));
							m.data[getParent(m, x)].color = MapInternal::BLACK;
							m.data[getRight(m, y)].color = MapInternal::BLACK;
							rotateLeft(m, getParent(m, x));
							x = m.root;
						}
					}
					else
					{
						y = getLeft(m, getParent(m, x));
						if (getColor(m, y) == MapInternal::RED)
						{
							m.data[y].color = MapInternal::BLACK;
							m.data[getParent(m, x)].color = MapInternal::RED;
							rotateRight(m, getParent(m, x));
							y = getLeft(m, getParent(m, x));
						}

						if (getColor(m, getRight(m, y)) == MapInternal::BLACK && getColor(m, getLeft(m, y)) == MapInternal::BLACK)
						{
							m.data[y].color = MapInternal::RED;
							x = getParent(m, x);
						}
						else
						{
							if (getColor(m, getLeft(m, y)) == MapInternal::BLACK)
							{
								m.data[getRight(m, y)].color = MapInternal::BLACK;
								m.data[y].color = MapInternal::RED;
								rotateLeft(m, y);
								y = getLeft(m, getParent(m, x));
							}

							m.data[y].color = getColor(m, getParent(m, x));
							m.data[getParent(m, x)].color = MapInternal::BLACK;
							m.data[getLeft(m, y)].color = MapInternal::BLACK;
							rotateRight(m, getParent(m, x));
							x = m.root;
						}
					}
				}

				m.data[x].color = MapInternal::BLACK;
			}

			destroy(m, y);
#ifdef RBTREE_VERIFY
			MapInternal::debugVerify(m, m.root);
#endif
		}

		template <typename TKey, typename TValue>
		void clear(Map<TKey, TValue>& m)
		{
			VectorFn::clear(m.data);

			m.root = 0;
			m.sentinel = 0;

			typename Map<TKey, TValue>::Node r;
			r.key = TKey();
			r.value = TValue();
			r.left = MapInternal::NIL;
			r.right = MapInternal::NIL;
			r.parent = MapInternal::NIL;
			r.color = MapInternal::BLACK;
			VectorFn::pushBack(m.data, r);
		}

		template <typename TKey, typename TValue>
		const typename Map<TKey, TValue>::Node* begin(const Map<TKey, TValue>& m)
		{
			return VectorFn::begin(m.data) + 1; // Skip sentinel at index 0
		}

		template <typename TKey, typename TValue>
		const typename Map<TKey, TValue>::Node* end(const Map<TKey, TValue>& m)
		{
			return VectorFn::end(m.data);
		}
	} // namespace MapFn

	template <typename TKey, typename TValue>
	inline Map<TKey, TValue>::Map(Allocator& a)
		: data(a)
	{
		MapFn::clear(*this);
	}

	template <typename TKey, typename TValue>
	inline const TValue& Map<TKey, TValue>::operator[](const TKey& key) const
	{
		return MapFn::get(*this, key, TValue());
	}

} // namespace Rio
