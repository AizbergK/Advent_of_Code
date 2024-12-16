#include "../../other/include_everything.h"
import std;

void read_data(std::pair<int, int>& size, std::vector<std::array<long long, 4>>& robots);
int part_one(std::pair<int, int>& size, std::vector<std::array<long long, 4>> robots);
int part_two(std::pair<int, int>& size, std::vector<std::array<long long, 4>> robots);

#pragma pack(push, 1)
struct BMPFileHeader {
     uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42
     uint32_t file_size{ 0 };               // Size of the file (in bytes)
     uint16_t reserved1{ 0 };               // Reserved, always 0
     uint16_t reserved2{ 0 };               // Reserved, always 0
     uint32_t offset_data{ 54 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
	uint32_t size{ 40 };                      // Size of this header (in bytes)
	int32_t width{ 0 };                      // width of bitmap in pixels
	int32_t height{ 0 };                     // width of bitmap in pixels
	                                         //       (if positive, bottom-up, with origin in lower left corner)
                                             //       (if negative, top-down, with origin in upper left corner)
	uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
	uint16_t bit_count{ 24 };                 // No. of bits per pixel
	uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	uint32_t size_image{ 0 };                // 0 - for uncompressed images
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

void writeBMP(const std::string& filename, int width, int height, const std::vector<uint8_t>& image)
{
	BMPFileHeader fileHeader;
	BMPInfoHeader infoHeader;

	infoHeader.width = width;
	infoHeader.height = -height; // Negative to indicate the top-down DIB

	fileHeader.file_size += 54 + infoHeader.height * infoHeader.width * 3;

	std::ofstream file(filename, std::ios::binary);
	if (file) {
		file.write(reinterpret_cast<const char*>(&fileHeader), 14);
		file.write(reinterpret_cast<const char*>(&infoHeader), 40);
		file.write(reinterpret_cast<const char*>(image.data()), image.size());
	}
	else {
		std::cerr << "Could not open file for writing!" << std::endl;
	}
}

int main()
{
	std::pair<int, int> size;
	std::vector<std::array<long long, 4>> robots;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 222.50µs  : data in + parsing
	// 14.700µs  : part1
	// 3.7108s   : part2 + 1 min manual searching images
	input_timer.startTimer();
	read_data(size, robots);
	input_timer.getDuration(u8"input day14");

	part1_timer.startTimer();
	part_one_result = part_one(size, robots);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(size, robots);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::pair<int, int>& size, std::vector<std::array<long long, 4>>& robots)
{
	int count{ 0 };
	std::ifstream input(__INPUT_PATH);
	std::string line;
	while (std::getline(input, line))
	{
		std::array<long long, 4> temp;
		temp[0] = std::stoll(line.substr(line.find_first_of('=') + 1));
		temp[1] = std::stoll(line.substr(line.find_first_of(',') + 1));
		temp[2] = std::stoll(line.substr(line.find_last_of('=') + 1));
		temp[3] = std::stoll(line.substr(line.find_last_of(',') + 1));
		robots.emplace_back(temp);
		++count;
	}
	size = count < 20 ? std::pair<int, int>{ 11, 7 } : std::pair<int, int>{ 101, 103 };

}

void print_bathroom(std::pair<int, int> size, std::vector<std::array<long long, 4>>& robots, int step)
{
	auto [width, height]{ size };
	std::vector<std::string> map{ std::vector<std::string>{ (unsigned)height, std::string(width, 0) } };
	for (int i{ 0 }; i != robots.size(); ++i)
	{
		map[robots[i][1]][robots[i][0]] = 1;
	}
	int div_four_width = width - 1;
	std::vector<uint8_t> image(div_four_width * height * 3, 0);

	// Convert matrix to BMP pixel data (0 -> black, 1 -> white)
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < div_four_width; ++x) {
			int pixel = map[y][x] * 255;
			image[(y * div_four_width + x) * 3 + 0] = pixel;
			image[(y * div_four_width + x) * 3 + 1] = pixel;
			image[(y * div_four_width + x) * 3 + 2] = pixel;
		}
	}
	writeBMP("./images/" + std::to_string(step + 1) + ".bmp", div_four_width, height, image);
}

void update_position(std::pair<int, int>& size,std::array<long long, 4>& robot, int seconds)
{
	robot[0] = (robot[0] + robot[2] * seconds) % size.first;
	robot[1] = (robot[1] + robot[3] * seconds) % size.second;
	if (robot[0] < 0) robot[0] += size.first;
	if (robot[1] < 0) robot[1] += size.second;
}

int part_one(std::pair<int, int>& size, std::vector<std::array<long long, 4>> robots)
{
	int result{ 0 };
	int first_tot{ 0 }, second_tot{ 0 }, third_tot{ 0 }, fourth_tot{ 0 };

	for (int i{ 0 }; i != robots.size(); ++i)
	{
		update_position(size, robots[i], 100);
	}

	for (int i{ 0 }; i != robots.size(); ++i)
	{
		if (robots[i][0] < size.first / 2 && robots[i][1] < size.second / 2)
			++first_tot;
		if (robots[i][0] > size.first / 2 && robots[i][1] < size.second / 2)
			++second_tot;
		if (robots[i][0] < size.first / 2 && robots[i][1] > size.second / 2)
			++third_tot;
		if (robots[i][0] > size.first / 2 && robots[i][1] > size.second / 2)
			++fourth_tot;
	}
	result = first_tot * second_tot * third_tot * fourth_tot;

	return result;
}

int part_two(std::pair<int, int>& size, std::vector<std::array<long long, 4>> robots)
{
	int result{ 0 };

	for (auto n{ 0 }; n != 10'000; ++n)
	{
		for (int i{ 0 }; i != robots.size(); ++i)
		{
			update_position(size, robots[i], 1);
		}
		print_bathroom(size, robots, n);
	}

	return result;
}