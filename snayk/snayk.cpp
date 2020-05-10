#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <math.h>

class Grid
{
	struct position {
		int pos_x{ 0 };
		int pos_y{ 0 };
	};
private:
	int row_x;
	int column_y;
	int slept{};

	bool check_if_outofmap(std::vector<position>& tail) {
		if (tail[0].pos_x < 0 || tail[0].pos_y < 0 || tail[0].pos_y >= row_x || tail[0].pos_x >= column_y) {
			return true;
		}
		else {
			return false;
		}
	}

	bool check_if_overlaps(std::vector<position>& tail) {
		std::size_t size{ tail.size() };
		if (size <= 1) { return false; }
		for (std::size_t i{ 1 }; i != size; i++) {
			if (tail[0].pos_x == tail[i].pos_x && tail[0].pos_y == tail[i].pos_y) {
				return true;
			}
		}
		return false;
	}

	position createNewYem() {
		position pos_yem{ getRandomNumber(0, row_x - 1), getRandomNumber(0, column_y - 1) };
		return pos_yem;
	}

	int getRandomNumber(int min, int max)
	{
		std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
		std::uniform_int_distribution die{ min, max };
		return die(mersenne);
	}

	char getKeyPress(int time_to_sleep)
	{
		for (int i{ 0 }; i != 10; i++) {
			if (_kbhit()) {
				char key = 127;
				key = _getch();
				if (key == 0 || key == -32)
				{
					key = _getch();
					if (key == 72) { key = 'u'; }
					else if (key == 75) { key = 'l'; }
					else if (key == 77) { key = 'r'; }
					else if (key == 80) { key = 'd'; }
				}
				return key;
			}
			else {
				slept = 100.0 - std::sqrt(300*time_to_sleep);
				Sleep(slept);
			}
		}
		return -1;
	}

	void moveTail(std::vector<position>& tail) {
		std::size_t size{ tail.size() };
		if (size > 1) {
			for (std::size_t i{ size - 1 }; i != 0; i--) {
				tail[i] = { tail[i - 1].pos_x, tail[i - 1].pos_y };
			}
		}
	}
	bool is_tail(std::vector<position>& tail, int row, int column) {
		for (position &pos : tail) {
			if (pos.pos_x == column && pos.pos_y == row) {
				return true;
			}
		}
		return false;
	}

	bool yem_spawned_overlap(std::vector<position>& tail, position &pos_yem) {
		for (auto& pos : tail) {
			if (pos.pos_x == pos_yem.pos_x && pos.pos_y == pos_yem.pos_y) {
				return true;
			}
		}
		return false;
	}

	void print_lose() {
		system("cls");
		std::cout << std::endl << std::endl << "\t\tYOU DIEDED" << std::endl << std::endl;
		system("pause");
	}

	void draw(position &pos_yem, std::vector<position> &tail, char add_tail_dir=NULL) {
		system("cls"); //sorry *nix users but it seemed so simple :(
		for (int row{ 0 }; row != row_x; row++) {
			for (int column{ 0 }; column != column_y; column++) {
				if (is_tail(tail, row, column)) {
					std::cout << "0" << "    ";
				}
				else if (pos_yem.pos_y == row && pos_yem.pos_x == column) {
					std::cout << "p" << "    ";
				}
				else {
					std::cout << ".    ";
				}
			}
			std::cout << std::endl << std::endl;
		}
		std::cout << std::endl << "player(x:" << tail[0].pos_x << ", y:" << tail[0].pos_y << ")" << std::endl;
		std::cout << "tail:";
		for (position& pos : tail) {
			std::cout << "(" << pos.pos_x << "," << pos.pos_y << ")";
		}
		std::cout << std::endl << "yem(x:" << pos_yem.pos_x << ", y:" << pos_yem.pos_y << ")" << std::endl;
		std::cout << slept*10 << "ms" << std::endl;
		if (tail[0].pos_x == pos_yem.pos_x && tail[0].pos_y == pos_yem.pos_y) { 
			pos_yem = createNewYem();
			if (!yem_spawned_overlap(tail, pos_yem)) {
				position new_tail{ tail[tail.size() - 1].pos_x,tail[tail.size() - 1].pos_y };
				tail.push_back(new_tail);
			}
			draw(pos_yem, tail);
		}
	}
public:
	Grid(int x = 3, int y = 3) : row_x{ x }, column_y{ y } {};

	void mainloop() {
		position player_pos;
		position yem{ createNewYem() };
		std::vector<position> tail;
		tail.push_back(player_pos);
		draw(yem, tail);
		char direction_prev{ 'r' };
		while (true) {
			std::size_t tail_size{ tail.size() };
			char direction{getKeyPress(tail.size())};
			if (direction == -1) { direction = direction_prev; }
			direction_prev = direction;
			moveTail(tail);
			switch (direction) {
			case 'd':
				tail[0].pos_y++;
				break;
			case 'u':
				tail[0].pos_y--;
				break;
			case 'l':
				tail[0].pos_x--;
				break;
			case 'r':
				tail[0].pos_x++;
				break;
			default:
				break;
			}
			if (check_if_overlaps(tail) || check_if_outofmap(tail)) {
				print_lose();
				return;
			}
			draw(yem, tail);
		}
		return;
	}
};

int main()
{
	Grid table(12, 12);
	table.mainloop();
	return 0;
}