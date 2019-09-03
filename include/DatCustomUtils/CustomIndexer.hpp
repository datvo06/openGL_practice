#ifndef __DAT_CUSTOM__ITERATOR__
#define __DAT_CUSTOM__ITERATOR__
#include <iterator>
namespace DatCustom{
	namespace Iterator {
		template <typename T>
		struct iterator_extractor { typedef typename T::iterator type; };

		template <typename T>
		struct iterator_extractor<T const> { typedef typename T::const_iterator type; };


		template <typename T>
		class Indexer {
		public:
				class iterator {
						typedef typename iterator_extractor<T>::type inner_iterator;

						typedef typename std::iterator_traits<inner_iterator>::reference inner_reference;
				public:
						typedef std::pair<size_t, inner_reference> reference;

						iterator(inner_iterator it): _pos(0), _it(it) {}

						reference operator*() const { return reference(_pos, *_it); }

						iterator& operator++() { ++_pos; ++_it; return *this; }
						iterator operator++(int) { iterator tmp(*this); ++*this; return tmp; }

						bool operator==(iterator const& it) const { return _it == it._it; }
						bool operator!=(iterator const& it) const { return !(*this == it); }

				private:
						size_t _pos;
						inner_iterator _it;
				};

				Indexer(T& t): _container(t) {}

				iterator begin() const { return iterator(_container.begin()); }
				iterator end() const { return iterator(_container.end()); }

		private:
				T& _container;
		}; // class Indexer

		template <typename T>
		Indexer<T> index(T& t) { return Indexer<T>(t); }
	}
}
#endif
