#pragma warning(push)
#pragma warning(disable: 4702)  // unreachable code
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#pragma warning(pop)

using namespace std::literals;

inline static constexpr std::uint32_t MAGICK = 0x434B414C;
inline static constexpr std::uint32_t CURRENT_VERSION = 0x1;

class HashUtils {
public:
	static std::uint32_t CRC32Buffer(const void* in, const std::uint32_t size) noexcept(true)
	{
		std::uint32_t crc = 0xFFFFFFFFul;
		const std::uint8_t* buf = (const std::uint8_t*)in;

		for (std::uint32_t i = 0; i < size; ++i)
			crc = crc_table[(crc ^ buf[i]) & 0xFF] ^ (crc >> 8);

		return ~crc;
	}

private:
	static constexpr uint32_t crc_table[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
	};
};

class Version
{
public:
	using value_type = std::uint16_t;
	using reference = value_type&;
	using const_reference = const value_type&;

	constexpr Version() noexcept = default;

	constexpr Version(std::array<value_type, 4> a_version) noexcept :
		_impl(a_version)
	{}

	[[nodiscard]] constexpr std::size_t size() const noexcept { return _impl.size(); }

	[[nodiscard]] constexpr reference       operator[](std::size_t a_idx) noexcept { return _impl[a_idx]; }
	[[nodiscard]] constexpr const_reference operator[](std::size_t a_idx) const noexcept { return _impl[a_idx]; }

	[[nodiscard]] int constexpr compare(const Version& a_rhs) const noexcept
	{
		for (std::size_t i = 0; i < _impl.size(); ++i) {
			if ((*this)[i] != a_rhs[i]) {
				return (*this)[i] < a_rhs[i] ? -1 : 1;
			}
		}
		return 0;
	}

	[[nodiscard]] std::string string() const
	{
		std::string result;
		for (std::size_t i = 0; i < _impl.size(); ++i) {
			result += std::to_string(_impl[i]);
			result += '-';
		}
		result.pop_back();
		return result;
	}

	[[nodiscard]] std::wstring wstring() const
	{
		std::wstring result;
		for (std::size_t i = 0; i < _impl.size(); ++i) {
			result += std::to_wstring(_impl[i]);
			result += L'-';
		}
		result.pop_back();
		return result;
	}

private:
	std::array<value_type, 4> _impl{ 0, 0, 0, 0 };
};

[[nodiscard]] constexpr bool operator==(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) == 0; }
[[nodiscard]] constexpr bool operator!=(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) != 0; }
[[nodiscard]] constexpr bool operator<(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) < 0; }
[[nodiscard]] constexpr bool operator<=(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) <= 0; }
[[nodiscard]] constexpr bool operator>(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) > 0; }
[[nodiscard]] constexpr bool operator>=(const Version& a_lhs, const Version& a_rhs) noexcept { return a_lhs.compare(a_rhs) >= 0; }

class Mapping
{
public:
	[[nodiscard]] constexpr bool assigned() const noexcept { return static_cast<bool>(_id); }

	void assign(std::uint32_t a_id) noexcept
	{
		_id = a_id;
		for (auto& link : _links) {
			if (!link->assigned()) {
				link->assign(a_id);
			}
		}
	}

	[[nodiscard]] constexpr std::uint32_t id() const noexcept
	{
		assert(assigned());
		return *_id;
	}

	void link(Mapping* a_mapping) { _links.insert(a_mapping); }

private:
	std::set<Mapping*>           _links;
	std::optional<std::uint32_t> _id;
};

using files_t = std::vector<std::tuple<Version, Version, std::filesystem::path>>;

[[nodiscard]] files_t get_files(const std::filesystem::path& a_root)
{
	files_t     results;
	std::wregex regex(L"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)_(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\\.txt"s, std::regex::ECMAScript);
	for (const auto& entry : std::filesystem::directory_iterator(a_root)) {
		if (entry.is_regular_file()) {
			const auto   filename = entry.path().filename();
			std::wsmatch matches;
			if (std::regex_match(filename.native(), matches, regex) && matches.size() == 9) {
				results.emplace_back();
				auto& [lversion, rversion, path] = results.back();

				const auto extract = [&]<std::size_t I>(std::in_place_index_t<I>) {
					return static_cast<std::uint16_t>(std::stoull(matches[I]));
				};

				lversion[0] = extract(std::in_place_index<1>);
				lversion[1] = extract(std::in_place_index<2>);
				lversion[2] = extract(std::in_place_index<3>);
				lversion[3] = extract(std::in_place_index<4>);

				rversion[0] = extract(std::in_place_index<5>);
				rversion[1] = extract(std::in_place_index<6>);
				rversion[2] = extract(std::in_place_index<7>);
				rversion[3] = extract(std::in_place_index<8>);

				path = entry.path();
			}
		}
	}
	return results;
}

using offset_map = std::unordered_map<std::uint32_t, Mapping>;
using version_map = std::map<Version, offset_map>;

[[nodiscard]] version_map load_mappings(const files_t& a_files)
{
	version_map       map;
	std::ifstream     file;
	std::stringstream ss;
	std::string       line;
	std::string       lbuf;
	std::string       rbuf;

	const auto find_or_emplace_version = [&map](const Version& a_version) -> offset_map& {
		auto it = map.find(a_version);
		if (it == map.end()) {
			it = map.emplace(a_version, offset_map()).first;
		}
		return it->second;
	};

	const auto find_or_emplace_address = [](offset_map& a_map, std::string a_address) -> Mapping& {
		auto address = static_cast<std::uint32_t>(std::stoull(a_address, 0, 16));
		address -= static_cast<std::uint32_t>(0x140000000);
		auto it = a_map.find(address);
		if (it == a_map.end()) {
			it = a_map.emplace(address, Mapping()).first;
		}
		return it->second;
	};

	for (const auto& [lver, rver, path] : a_files) {
		file.open(path);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file for read"s);
		}
		for (std::size_t i = 0; i < 18; ++i) {
			std::getline(file, line);  // skip header
		}

		auto& laddr = find_or_emplace_version(lver);
		auto& raddr = find_or_emplace_version(rver);

		while (std::getline(file, line)) {
			ss.str(line);
			ss.clear();
			std::getline(ss, lbuf, '\t');
			std::getline(ss, rbuf, '\t');
			if (ss && !lbuf.empty() && !rbuf.empty()) {
				auto& lmap = find_or_emplace_address(laddr, lbuf);
				auto& rmap = find_or_emplace_address(raddr, rbuf);
				lmap.link(std::addressof(rmap));
				rmap.link(std::addressof(lmap));
			}
		}

		file.close();
	}

	return map;
}

void assign_ids(version_map& a_versionMap)
{
	std::uint32_t id = 1;
	for (auto& [ver, offsetMap] : a_versionMap) {
		for (auto& [offset, mapping] : offsetMap) {
			if (!mapping.assigned()) {
				mapping.assign(id++);
			}
		}
	}
}

void write_binaries(version_map& a_versionMap)
{
	std::ofstream file;
	const auto    binary_write = [&file](auto&& a_data) {
        file.write(
            reinterpret_cast<const char*>(std::addressof(a_data)),
            sizeof(std::remove_reference_t<decltype(a_data)>));
	};

	std::string                                    filename;
	std::vector<std::pair<std::uint32_t, std::uint32_t>> mappings;
	for (const auto& [ver, offsetMap] : a_versionMap) {
		filename = "version-"sv;
		filename += ver.string();
		filename += ".bin"sv;
		file.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file for write"s);
		}

		mappings.clear();
		mappings.reserve(offsetMap.size());
		for (const auto& [offset, mapped] : offsetMap) {
			mappings.emplace_back(mapped.id(), offset);
		}
		std::sort(
			mappings.begin(),
			mappings.end(),
			[](auto&& a_lhs, auto&& a_rhs) {
				return a_lhs.first < a_rhs.first;
			});

		std::uint32_t crc = HashUtils::CRC32Buffer(
			mappings.data(),
			static_cast<std::uint32_t>(mappings.size() * sizeof(mappings[0]))
		);

		binary_write(MAGICK);
		binary_write(CURRENT_VERSION);
		binary_write(static_cast<std::uint32_t>(crc));
		binary_write(static_cast<std::uint32_t>(mappings.size()));
		for (const auto& [id, offset] : mappings) {
			binary_write(id);
			binary_write(offset);
		}

		file.close();
	}
}

void load_unmatched(version_map& a_versionMap)
{
	const auto find_or_emplace_address = [](offset_map& a_map, std::string a_address) -> Mapping& {
		auto address = static_cast<std::uint32_t>(std::stoull(a_address, 0, 16));
		address -= static_cast<std::uint32_t>(0x140000000);
		auto it = a_map.find(address);
		if (it == a_map.end()) {
			it = a_map.emplace(address, Mapping()).first;
		}
		return it->second;
	};

	for (auto& [ver, offsetMap] : a_versionMap) {
		std::ifstream     file;
		std::string filename;
		for (std::size_t i = 0; i < ver.size(); ++i) {
			filename += std::to_string(ver[i]);
			filename += '.';
		}
		filename.pop_back();
		filename += ".txt"sv;

		std::filesystem::path path = std::filesystem::path("unmatched") / filename;
		file.open(path);
		if (!file.is_open()) {
			continue;
		}

		std::string       line;
		while (std::getline(file, line)) {
			if (line.empty()) {
				continue;
			}

			find_or_emplace_address(offsetMap, line);
		}

		file.close();
	}
}

int main()
{
	try {
		auto mappings = load_mappings(get_files("mappings"sv));
		load_unmatched(mappings);
		assign_ids(mappings);
		write_binaries(mappings);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
