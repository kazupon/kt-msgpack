#ifndef MPRPC_KyotoTycoonService_4c409fcd_HPP__
#define MPRPC_KyotoTycoonService_4c409fcd_HPP__

#include "types.hpp"


namespace KyotoTycoonService {


struct ping {

	ping()
	{
	}


	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(0);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 0) {
			throw msgpack::type_error();
		}

	}
};
struct echo {

	echo()
	{
	}

	std::map<msgpack::type::raw_ref,msgpack::type::raw_ref>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(1);
		_Pk.pack(inmap);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 1) {
			throw msgpack::type_error();
		}


			_Array[0].convert(&inmap);

	}
};
struct report {

	report()
	{
	}


	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(0);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 0) {
			throw msgpack::type_error();
		}

	}
};
struct status {

	status()
	{
	}

	msgpack::type::raw_ref DB;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(1);
		_Pk.pack(DB);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 0) {
			throw msgpack::type_error();
		}


			if(_Length <= 0) { return; }
			if(!_Array[0].is_nil()) {
				_Array[0].convert(&DB);
			}

	}
};
struct add {

	add()
		:
		xt(C_INT64_MAX)
	{
	}

	msgpack::type::raw_ref key;
	msgpack::type::raw_ref value;
	msgpack::type::raw_ref DB;
	int64_t xt;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(4);
		_Pk.pack(key);
		_Pk.pack(value);
		_Pk.pack(DB);
		_Pk.pack(xt);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 2) {
			throw msgpack::type_error();
		}


			_Array[0].convert(&key);


			_Array[1].convert(&value);


			if(_Length <= 2) { return; }
			if(!_Array[2].is_nil()) {
				_Array[2].convert(&DB);
			}


			if(_Length <= 3) { return; }
			if(!_Array[3].is_nil()) {
				_Array[3].convert(&xt);
			}

	}
};
struct set {

	set()
		:
		xt(C_INT64_MAX)
	{
	}

	msgpack::type::raw_ref key;
	msgpack::type::raw_ref value;
	msgpack::type::raw_ref DB;
	int64_t xt;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(4);
		_Pk.pack(key);
		_Pk.pack(value);
		_Pk.pack(DB);
		_Pk.pack(xt);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 2) {
			throw msgpack::type_error();
		}


			_Array[0].convert(&key);


			_Array[1].convert(&value);


			if(_Length <= 2) { return; }
			if(!_Array[2].is_nil()) {
				_Array[2].convert(&DB);
			}


			if(_Length <= 3) { return; }
			if(!_Array[3].is_nil()) {
				_Array[3].convert(&xt);
			}

	}
};
struct get {

	get()
	{
	}

	msgpack::type::raw_ref key;
	msgpack::type::raw_ref DB;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(2);
		_Pk.pack(key);
		_Pk.pack(DB);
	}

	void msgpack_unpack(msgpack::object _Obj) {
		if(_Obj.type != msgpack::type::ARRAY) {
			throw msgpack::type_error();
		}
		const size_t _Length = _Obj.via.array.size;
		msgpack::object* const _Array = _Obj.via.array.ptr;

		if(_Length < 1) {
			throw msgpack::type_error();
		}


			_Array[0].convert(&key);


			if(_Length <= 1) { return; }
			if(!_Array[1].is_nil()) {
				_Array[1].convert(&DB);
			}

	}
};


}  // namespace KyotoTycoonService

#endif
