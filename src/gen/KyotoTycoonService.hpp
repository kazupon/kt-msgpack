#ifndef MPRPC_KyotoTycoonService_bb99f0e3_HPP__
#define MPRPC_KyotoTycoonService_bb99f0e3_HPP__

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

	std::map<std::string,std::string>  inmap;

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

		if(_Length < 0) {
			throw msgpack::type_error();
		}


			if(_Length <= 0) { return; }
			if(!_Array[0].is_nil()) {
				_Array[0].convert(&inmap);
			}

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

	std::map<std::string,std::string>  inmap;

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

		if(_Length < 0) {
			throw msgpack::type_error();
		}


			if(_Length <= 0) { return; }
			if(!_Array[0].is_nil()) {
				_Array[0].convert(&inmap);
			}

	}
};
struct add {

	add()
	{
	}

	std::string key;
	std::string value;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(3);
		_Pk.pack(key);
		_Pk.pack(value);
		_Pk.pack(inmap);
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
				_Array[2].convert(&inmap);
			}

	}
};
struct set {

	set()
	{
	}

	std::string key;
	std::string value;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(3);
		_Pk.pack(key);
		_Pk.pack(value);
		_Pk.pack(inmap);
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
				_Array[2].convert(&inmap);
			}

	}
};
struct get {

	get()
	{
	}

	std::string key;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(2);
		_Pk.pack(key);
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


			_Array[0].convert(&key);


			if(_Length <= 1) { return; }
			if(!_Array[1].is_nil()) {
				_Array[1].convert(&inmap);
			}

	}
};
struct remove {

	remove()
	{
	}

	std::string key;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(2);
		_Pk.pack(key);
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


			_Array[0].convert(&key);


			if(_Length <= 1) { return; }
			if(!_Array[1].is_nil()) {
				_Array[1].convert(&inmap);
			}

	}
};
struct append {

	append()
	{
	}

	std::string key;
	std::string value;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(3);
		_Pk.pack(key);
		_Pk.pack(value);
		_Pk.pack(inmap);
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
				_Array[2].convert(&inmap);
			}

	}
};
struct seize {

	seize()
	{
	}

	std::string key;
	std::map<std::string,std::string>  inmap;

	template <typename Packer>
	void msgpack_pack(Packer& _Pk) const {
		_Pk.pack_array(2);
		_Pk.pack(key);
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


			_Array[0].convert(&key);


			if(_Length <= 1) { return; }
			if(!_Array[1].is_nil()) {
				_Array[1].convert(&inmap);
			}

	}
};


}  // namespace KyotoTycoonService

#endif
