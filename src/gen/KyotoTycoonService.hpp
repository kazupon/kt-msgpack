#ifndef MPRPC_KyotoTycoonService_e1c938df_HPP__
#define MPRPC_KyotoTycoonService_e1c938df_HPP__

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


}  // namespace KyotoTycoonService

#endif
