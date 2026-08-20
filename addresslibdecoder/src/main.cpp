#pragma warning(push)
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>

#include <mmio/mmio.hpp>
#pragma warning(pop)

using namespace std::literals;

struct Header
{
	inline static constexpr std::uint32_t MAGICK = 0x434B414C;
	inline static constexpr std::uint32_t CURRENT_VERSION = 0x1;

	std::uint32_t magick{ MAGICK };
	std::uint32_t version{ CURRENT_VERSION };
	std::uint32_t crc32{ (std::uint32_t)-1 };
	std::uint32_t count{ 0 };
};

struct Pair
{
	std::uint32_t id;
	std::uint32_t offset;
};

int main(int a_argc, char* a_argv[])
{
	try {
		mmio::mapped_file_source input;
		std::ofstream            output;

		for (int i = 1; i < a_argc; ++i) {
			std::filesystem::path filename = a_argv[static_cast<std::size_t>(i)];
			if (!input.open(filename.string())) {
				throw std::runtime_error("failed to open: "s + filename.string());
			}

			filename.replace_extension(".txt");
			output.open(filename.c_str(), std::ios::out | std::ios::trunc);
			if (!output.is_open()) {
				throw std::runtime_error("failed to open: "s + filename.string());
			}

			const Header* header = reinterpret_cast<const Header*>(input.data());
			std::span data(
				reinterpret_cast<const Pair*>(input.data() + sizeof(Header)),
				header->count);
			if (!data.empty()) {
				const auto width = std::format("{}", data.back().id);
				const auto format = std::format("{{: >{}}}\t{{:0>7X}}\n", width.length());
				for (const auto& [id, offset] : data) {
					output << std::vformat(format, std::make_format_args(id, offset));
				}
			}

			input.close();
			output.close();
		}
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
