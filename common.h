/*
 * Enum to represent the four directions
 * Here is an example of how to use an enum:
 *
 * Direction dir = EAST;
 * if (dir == EAST) {
 *     printf("Heading EAST!");
 * }
 */
typedef enum {
	NORTH = 0,
	SOUTH = 1,
	WEST = 2,
	EAST = 3
} Direction;

/*
 * Struct to represent a room in the maze.
 * Must store:
 * 	- The location of the room
 * 	- Whether or not this room has been visited
 *	- The connections to neighboring rooms in all
 *	  four directions.
 */
struct maze_room {
	int row;
	int col;
	int visited; //0 for not visited, 1 for visited
	int walls[4]; //1 for connection, 0 for opening, -1 for not initialized yet
	struct maze_room *next;
};


int get_index(int row, int col, int num_cols);

int is_in_range(int row, int col, int num_rows, int num_cols);

struct maze_room *get_neighbor(struct maze_room maze[], int num_rows,
        int num_cols, struct maze_room *room, Direction dir);

