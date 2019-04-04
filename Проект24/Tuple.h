#pragma once 

#include <iostream>
#include <cassert>
#include <utility>


namespace tuple_ {

	template <typename... Args>
	class Tuple;

	template <>
	class Tuple<> {
	public:
		void swap(Tuple<>& other) {};
	};

	template <typename T, typename... Args>
	class Tuple<T, Args...> : public  Tuple<Args...> {
	private:
		T _el;
		Tuple<Args...> _other;
		Tuple(T&& el, Tuple<Args...>&& other) : _el(std::move(el)), _other(std::move(other)) {};
	public:
		T _el;
		Tuple<Args...> _other;



		//Tuple(T&& el, Tuple<Args...>&& other) : _el(std::move(el)), _other(std::move(other)) {};

		Tuple() : _el(), _other() {};

		template <typename U, typename ...R>
		Tuple() : _el(), _other() {};

		Tuple(const T& el, const Args&... other) : _el(el), Tuple<Args...>(other...) {};

		T& get() { return _el; }

		T get_val() const { return _el; }

		Tuple<Args...>& next() { return *static_cast<Tuple<Args...>*>(this); }

		const Tuple<Args...>& next() const { return *static_cast<Tuple<Args...>*>(this); }

		template <typename U, typename... Args_U, typename = std::enable_if_t<sizeof...(Args) == sizeof...(Args_U) &&
			std::is_same<U, T>::value, T>::value_type >
		explicit Tuple(U&& el, Args_U&&... other) : _el(std::forward<U>(el)),
			Tuple<Args_U...>(std::forward<Args_U>(other)...) {};

		Tuple(const Tuple&) = default;
		Tuple(Tuple&&) = default;
			
		template <typename U, typename... Args_U, typename = std::enable_if_t<sizeof...(Args) == sizeof...(Args_U) > >
		Tuple(const Tuple<U, Args_U...>& other) : _el(tup_other.get()), 
			Tuple<Args...>(tup_other.next()) {};
		
		template <typename U, typename... Args_U, typename = std::enable_if_t<sizeof...(Args) == sizeof...(Args_U) > >
		Tuple(Tuple<U, Args_U...>&& tup_other) : _el(std::move(tup_other.get())),
			Tuple<Args...>(std::move(tup_other.next())) {};

		Tuple& operator= (Tuple&& tup_other) = default;

		Tuple &operator=(const Tuple &right) {
			_el = right._el;
			Tuple <Args...>::operator=(right);
			return *this;
		}

		template <typename U, typename... Args_U, typename = std::enable_if_t<sizeof...(Args) == sizeof...(Args_U) > >
		Tuple& operator= (const Tuple<U, Args_U...>& tup_other) {
			_el = tup_other.get();
			next() = tup_other.next();
			return *this;
		}

		template <typename U, typename... Args_U, typename = std::enable_if_t<sizeof...(Args) == sizeof...(Args_U) > >
		Tuple& operator= (Tuple<U, Args_U...>&& tup_other) {
			_el = std::move(tup_other.get());
			next() = std::move(tup_other.next());
			return *this;
		}

		~Tuple() = default;


		void swap(Tuple<T, Args...>& other) {
			std::swap(_el, other.get());
			next().swap(other.next());
		};

		constexpr static std::size_t size() {
			return 1 + sizeof...(Args);
		}
		template <typename T, typename... Args_T, typename... Args_U>
		decltype(auto) MergeTupleFirst(Tuple<T, Args_T...>&& t1, Tuple<Args_U...>&& t2) {
			return Tuple<T, Args_T..., Args_U...>(std::move(t1._el), MergeTupleSecond(std::move(t1._other), std::move(t1)));
		}

		template <typename T, typename... Args_T, typename... Args_U>
		decltype(auto) MergeTupleFirst(const Tuple<T, Args_T...>& t1, const Tuple<Args_U...>& t2) {
			return Tuple<T, Args_T..., Args_U...>(t1._el, MergeTupleSecond(t1._other, t2));
		}

		template <typename T, typename... Args_T, typename... Args_U>
		decltype(auto) MergeTupleFirst(const Tuple<T, Args_T...>& t1, Tuple<Args_U...>&& t2) {
			return Tuple<T, Args_T..., Args_U...>(t1._el, MergeTupleSecond(t1._other, std::move(t2)));
		}

		template <typename T, typename... Args_T, typename... Args_U>
		decltype(auto) MergeTupleFirst(Tuple<T, Args_T...>&& t1, const Tuple<Args_U...>& t2) {
			return Tuple<T, Args_T..., Args_U...>(std::move(t1._el), MergeTupleSecond(std::move(t1._other), t2));
		}

		template <typename T, typename U>
		decltype(auto) MergeTupleSecond(T&& t1, U&& t2) {
			return MergeTupleSecond(std::forward<T>(t1), std::forward<U>(t2));
		}

		template <typename T>
		decltype(auto) MergeTupleSecond(T&& t1, Tuple<> t2) {
			return std::forward<T>(t1);
		}
	};
	
	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<std::is_same<U, T>::value, const U&> get(const Tuple<T, Args...>& cur_tuple) { return cur_taple.get(); }

	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<!std::is_same<U, T>::value, const U&> get(const Tuple<T, Args...>& cur_tuple) { return get<U>(cur_tuple.next()); }

	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<!std::is_same<U, T>::value, U&> get(Tuple<T, Args...>& cur_tuple) { return get<U>(cur_tuple.next()); }

	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<std::is_same<U, T>::value, U&> get(Tuple<T, Args...>& cur_tuple) { return cur_tuple.get(); }

	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<std::is_same<U, T>::value, U&&> get(Tuple<T, Args...>&& cur_tuple) { return std::move(cur_tuple.get()); }

	template <typename U, typename T, typename... Args>
	typename std::enable_if_t<!std::is_same<U, T>::value, U&&> get(Tuple<T, Args...>&& cur_tuple) { return get<U>(std::move(cur_tuple.next())); }



	template <int n, typename T, typename... Args, typename  = std::enable_if_t<n == 0> >
	typename std::enable_if_t<n == 0, T&>  get(Tuple<T, Args...>& cur_tuple) { return cur_tuple.get(); }

	template <int n, typename T, typename... Args, typename = std::enable_if_t<n != 0> >
	decltype(auto) get(Tuple<T, Args...>& cur_tuple) { return get<n - 1>(cur_tuple.next()); }

	template <int n, typename T, typename... Args, typename = std::enable_if_t<n == 0> >
	typename std::enable_if_t<n == 0, const T&> get(const Tuple<T, Args...>& cur_tuple) { return std::remove cur_tuple.get(); }

	template <int n, typename T, typename... Args, typename  = std::enable_if_t<n != 0> >
	decltype(auto) get(const Tuple<T, Args...>& cur_tuple) { return get<n - 1>(cur_tuple.next()); }

	template <int n, typename T, typename... Args, typename  = std::enable_if_t<n == 0> > 
	typename std::enable_if_t<n == 0, T&&> get(Tuple<T, Args...>&& cur_tuple) { return std::move(cur_tuple.get()); }

	template <int n, typename T, typename... Args, typename = std::enable_if_t<n != 0> >
	decltype(auto) get(Tuple<T, Args...>&& cur_tuple) { return std::move(get<n - 1>(cur_tuple.next())); }


	template < typename T, typename... Args_T, typename U, typename... Args_U>
	std::enable_if_t< sizeof...(Args_T) != 0, bool > comp(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return t1.get_val() < t2.get_val() || (t1.get_val() == t2.get_val() && comp(t1.next(), t2.next()));
	};

	template < typename T, typename U>
	bool comp(const Tuple<T>& t1, const Tuple<U>& t2) {
		return t1.get() < t2.get();
	};

	template < typename T, typename... Args_T, typename U, typename... Args_U>
	typename std::enable_if_t< sizeof...(Args_T) != 0, bool > same(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		if (sizeof...(Args_T) != sizeof...(Args_U))
			return false;
		return t1.get_val() == t2.get_val() && same(t1.next(), t2.next());
	};

	template < typename T, typename U>
	bool same(const Tuple<T>& t1, const Tuple<U>& t2) {
		return t1.get_val() == t2.get_val();
	};

	template <typename T>
	struct make_tuple_return_impl {
		using type = T;
	};

	template <typename	T>
	struct make_tuple_return_impl<std::reference_wrapper<T>> {
		using type = T&;
	};

	template <typename T>
	struct make_tuple_return {
		using type = typename make_tuple_return_impl<typename std::decay<T>::type>::type;
	};

	template <typename... other>
	constexpr auto makeTuple(other&&... other_) {
		return Tuple<typename make_tuple_return<other>::type...>(std::forward<other>(other_)...);
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator <(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		if (t1.get_val() == t2.get_val() && (sizeof...(Args_U) == 0))
			return false;
		if (t1.get_val() == t2.get_val() && (sizeof...(Args_T) == 0))
			return true;
		return t1.get_val() < t2.get_val() || (t1.get_val() == t2.get_val() && comp(t1.next(), t2.next()));
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator ==(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return same(t1, t2);
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator !=(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return !(t1 == t2);
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator <=(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return (t1 < t2 || t1 == t2);
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator >(const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return !(t1 <= t2);
	};

	template <typename T, typename... Args_T, typename U, typename... Args_U>
	bool operator >= (const Tuple<T, Args_T...>& t1, const Tuple<U, Args_U...>& t2) {
		return !(t1 < t2);
	};

	template<typename T, typename U, typename... Other>
	decltype(auto) tupleCat(T&& first, U&& second, Other&&... other) {
		return MergeTupleSecond(std::forward<First>(first),
			std::move(tupleCat(std::forward<Second>(second), std::forward<Other>(other)...)));
	}

	template<typename T, typename U>
	decltype(auto) tupleCat(First&& first, Second&& second) {
		return MrgeTupleSecond(std::forward<T>(first), std::forward<U>(second));
	}

}
	