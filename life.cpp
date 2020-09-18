#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


class Life {
public:
	enum class States {
		Dead,
		Alive
	};
	Life(int xsize, int ysize) {
		WORLD_SIZE_X = xsize;
		WORLD_SIZE_Y = ysize;
		CurrentGeneration.resize(WORLD_SIZE_X);
		NextGeneration.resize(WORLD_SIZE_X);
		for (size_t i = 0; i < WORLD_SIZE_X; i++) {
			CurrentGeneration[i].resize(WORLD_SIZE_Y);
			NextGeneration[i].resize(WORLD_SIZE_Y);
		}
		setCellsNumber();
		genFirstGeneration();
	}
	void setCellsNumber() {
		std::random_device rd;
		std::mt19937 m(rd());
		CellsNumber = m() % (WORLD_SIZE_X * WORLD_SIZE_Y / 2);
	}
	void genFirstGeneration() {
		size_t total = 0;
		while (total != CellsNumber) {
			
			std::random_device rd;
			std::mt19937 m(rd());
			size_t x = m() % WORLD_SIZE_X;
			size_t y = m() % WORLD_SIZE_Y;
			total++;
			CurrentGeneration[x][y] = States::Alive;	
		}
	}
	void genNextGeneration() {
		for(int i = 0; i < WORLD_SIZE_X; i++) {
			for (int j = 0; j < WORLD_SIZE_Y; j++) {
				if (deathCondition(i,j)) {
					NextGeneration[i][j] = States::Dead;
					CellsNumber--;
				}
				else if (birthCondition(i,j)) {
					NextGeneration[i][j] = States::Alive;
					CellsNumber++;
				}
				else {
					NextGeneration[i][j] = CurrentGeneration[i][j];
				}
			}
		}
	}
	void liveLife() {
		bool first_run = true;
		while(!isGameOver()) {
			if (!first_run) {
				CurrentGeneration = NextGeneration;
			}
			printUniverse();				
			std::this_thread::sleep_for(std::chrono::seconds(5));
			genNextGeneration();
			first_run = false;
		}
	}
	size_t getNeighboursCount(const int& x, const int& y) const {
		size_t count = 0;
		int x_start = x - 1;
		int x_last = x + 1;
		int y_start = y - 1;
		int y_last = y + 1;

		if (0 == x) {
			x_start = WORLD_SIZE_X - 1;
		}
		if (WORLD_SIZE_X - 1 == x) {
			x_last = 0;	
		}
		if (0 == y) {
			y_start = WORLD_SIZE_Y - 1;
		}
		if (WORLD_SIZE_Y - 1 == y) {
			y_last = 0;	
		}
		for (int i = x_start; i < x_last; i++) {
			for (int j = y_start; j < y_last; j++) {
				if (i != j and CurrentGeneration[i][j] == States::Alive) {
					count++;
				}
			}
		}
		return count;
	}
	bool deathCondition(const int& x, const int& y) const {
		return getNeighboursCount(x,y) < 2 or getNeighboursCount(x,y) > 3;
	}
		
	bool birthCondition(const int& x, const int& y) const {
		return getNeighboursCount(x,y) == 3;
	}
		
	bool isGameOver() const {
		if (CurrentGeneration == NextGeneration) {
			return true;
		}	
		if (CellsNumber == 0) {
			return true;
		}
		return false;
	}
	void printUniverse() const {
		printBorder();
		for (const auto& row : CurrentGeneration) {
			std::cout << '|';
			for (const auto& c : row) {
				std::cout << RED << printCell(c) << RESET;
			}
			std::cout << '|' << std::endl;
		}	
		printBorder();
	}
	char printCell(const Life::States& st) const {
		if (st == States::Dead) {
			return dead;
		}
		else {
			return cell;
		}
	}
	void printBorder() const {
		std::cout << ' ';
		for (size_t i = 0; i < WORLD_SIZE_Y; i++) {
			std::cout << '=';
		}
		std::cout << ' ' << std::endl;
	}
	void clearPrint() {

	}
private:
	size_t CellsNumber;
	const char dead = ' ';
	const char cell = '*';
	std::vector<std::vector<States> > CurrentGeneration;
	std::vector<std::vector<States> > NextGeneration;
	size_t WORLD_SIZE_X;
	size_t WORLD_SIZE_Y;
};
auto main(int argc, char* argv[]) -> int {
	Life life(std::atoi(argv[1]), std::atoi(argv[2]));
	life.liveLife();	
	return 0;
}
