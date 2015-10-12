#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

/*
 * Gets the index of a one-dimensional array that corresponds to the given row 
 * and column in a two-dimensional grid. This is used to convert from a two-dimensional
 * coordinate to a one-dimensional array index. If you are using two-dimensional arrays,
 * you do not need this function - feel free to comment it out.
 *
 * Parameters:
 *	- row: row of the room being queried
 *	- col: column of the room being queried
 *	- num_cols: number of columns in the maze
 * 
 * Returns:
 *	- the index in the one dimensional array that corresponds to the given 
 *	  row and column
 */
int get_index(int row, int col, int num_cols) {
	return row*num_cols + col;
}

/*
 * Determines whether or not the room at [row][col] is a valid room within the maze with
 * dimensions num_rows x num_cols
 *
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 * 
 * Returns:
 *	- 0 if room is not in the maze, 1 if room is in the maze
 */
int is_in_range(int row, int col, int num_rows, int num_cols) {
	if(row >= 0 && row < num_rows && col >= 0 && col < num_cols){
      return 1;
    } else {
      return 0;
    }
}

/*
 * Given the dimensions of a maze, a room, and a neighboring direction,
 * returns 1 if the neighboor in the specifed direction is out of bounds, 0
 * if it is in bounds
 *
 * Parameters:
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- room: pointer to the current room
 *	- dir: Direction to get the neighbor from
 *
 * Returns:
 *	- 1 if the neighboor is out of bounds, 0 otherwise
 */
int neighbor_out_bound(int num_rows,
		int num_cols, struct maze_room *room, Direction dir){
	int row = room->row;
	int col = room->col;
	

	if(dir == NORTH){

		if(row>0){
			return 0;
		}else {
			return 1;
		}

	} else if(dir == EAST){

		if(col<num_cols-1){
			return 0;
		}else{
			return 1;
		}

	} else if(dir == SOUTH){

		if(row<num_rows-1){
			return 0;
		}else{
			return 1;
		}
		

	} else {

		if(col>0){
			return 0;
		}else{
			return 1;
		}

	}
}

/*
 * Given a pointer to the room and a Direction to travel in, return a pointer to the room
 * that neighbors the current room on the given Direction.
 * For example: get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *	- maze: an array of maze_room structs representing your maze
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- room: pointer to the current room
 *	- dir: Direction to get the neighbor from
 *
 * Returns:
 *	- pointer to the neighboring room. If the neighboring room is out
 *	  of bounds of the maze, return NULL
 */
struct maze_room *get_neighbor(struct maze_room maze[], int num_rows,
		int num_cols, struct maze_room *room, Direction dir) {
	
	int row = room->row;
	int col = room->col;
	

	if(dir == NORTH){

		assert(row>0);
		int north_index = get_index(row-1, col, num_cols);
		return &maze[north_index]; //IS THIS HOW YOU DO IT??

	} else if(dir == EAST){

		assert(col<num_cols-1);
		int east_index = get_index(row, col+1, num_cols);
		return &maze[east_index];

	} else if(dir == SOUTH){

		assert(row<num_rows-1);
		int south_index = get_index(row+1, col, num_cols);
		return &maze[south_index];

	} else {

		assert(col>0);
		int west_index = get_index(row, col-1, num_cols);
		return &maze[west_index];

	}
}
/*
 * Given a pointer to a maze_room, set its connections in all four directions based
 * on the hex value passed in.
 *
 * For example:
 *
 * 		create_room_connections(&maze[3][4], 0xb)
 *
 * 0xb is 1011 in binary, which means we have walls on all sides except the EAST.
 * This means that every connection in maze[3][4] should be set to 1, except the EAST
 * connection, which should be set to 0.
 *
 * See the handout for more details about our hexadecimal representation, as well as
 * examples on how to use bit masks to extract information from this hexadecimal
 * representation.
 *
 * Parameters:
 *	- room: pointer to the current room
 *	- hex: hexadecimal integer (between 0-15 inclusive) that represents the connections
 *		   in all four directions from the given room.
 *
 * Returns:
 *	- nothing. The connections should be saved in the maze_room struct pointed to by
 *	  the parameter (make sure to use pointers correctly!).
 */
void create_room_connections(struct maze_room *room, unsigned int hex) {
	
	if(hex <= 7){
		room->walls[3] = 0;
	} else {
		room->walls[3] = 1;
	}

	if(hex <= 3 || (hex >= 8 && hex <= 11)){
		room->walls[2] = 0;
	} else {
		room->walls[2] = 1;
	}

	if(hex==0 || hex==1 || hex==4 || hex==5 || hex==8 || hex==9 ||
	   hex==12 || hex==13){
		room->walls[1] = 0;
	} else{
		room->walls[1] = 1;
	}

	if(hex % 2 == 0){
		room->walls[0] = 0;
	} else {
		room->walls[0] = 1;
	}
	
}

void print_maze(struct maze_room maze[], int num_rows, int num_cols){
	int r; int c;
	for(r=0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			int i = get_index(r, c, num_cols);
			printf("(%d, %d) ", r, c);
			printf("[%d, %d, %d, %d] \n", 
					maze[i].walls[0], maze[i].walls[1], maze[i].walls[2], maze[i].walls[3]);
		}
	}
}

/*
 * Recursive depth-first search algorithm for solving your maze.
 * This function should also print out either every visited room as it goes, or
 * the final pruned solution, depending on whether the FULL macro is set.
 * 
 * Note that you may need to add extra parameters to this function's signature,
 * depending on how you choose to store the pruned solution.
 *
 * See handout for more details, as well as a pseudocode implementation.
 *
 * Parameters:
 *	- row: row of the current room
 *	- col: column of the current room
 *	- goal_row: row of the goal room
 *	- goal_col: col of the goal room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- maze: an array of maze_room structs representing your maze
 *	- file: the file to write the solution to
 *  - full: 1 to print full path, 0 to print prunned path 
 *
 * Returns:
 *	- 1 if the current branch finds a valid solution, 0 if no branches are valid.
 */
int dfs(int row, int col, int goal_row, int goal_col, int num_rows,
		int num_cols, struct maze_room maze[], FILE *file, int full) {

	if(row == goal_row && col == goal_col){
		return 1;
	}

	//printf("dfs(%d, %d)\n", row, col);


	fprintf(file, "(%d, %d)\n", row, col);
	int index = get_index(row, col, num_cols);

	maze[index].visited = 1;

	Direction dirs[4] = {NORTH, SOUTH, WEST, EAST};
	int i;
	for(i=0; i<4; i++){

		

		int neighbor_out = neighbor_out_bound(num_rows,
								num_cols, &maze[index], i);

		if(neighbor_out == 0){
			

			struct maze_room *neighbor = get_neighbor(maze,num_rows, num_cols, 
												  &maze[index], i);

			if(maze[index].walls[i] == 0 && neighbor->visited == 0){
				//printf("   at (%d, %d), visiting neighbor at dir: %d \n", row, col, i);
				int neighbor_path = dfs(neighbor->row, neighbor->col, goal_row,
									goal_col, num_rows, num_cols, maze, file, full);
				if(neighbor_path == 1){
					
					printf("setting neighboor from index %d", index);
					maze[index].next = neighbor;

					return 1;
				} else {
					if(full == 1){
						printf("setting neighboor from index %d", index);
						maze[index].next = neighbor;
					}
				}
			}
		}
		

	}



	return 0;
}

/*
 * Main function
 *
 * Parameters:
 *	- argc: the number of command line arguments - for this function 9
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["solver", <input maze file>, <number of rows>, <number of columns>
 *				 <output solution file>, <starting row>, <starting column>, 
 *				 <ending row>, <ending column>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */
int main(int argc, char **argv) {
	char *maze_file;
	int num_rows;
	int num_cols;
	char *path_file;
	int start_row;
	int start_col;
	int goal_row;
	int goal_col;
	
	if (argc != 9) {
		printf("Incorrect number of arguments.\n");
		printf("./solver <input maze file> <number of rows>");
		printf(" <number of columns> <output solution file> <starting row>");
		printf(" <starting column> <ending row> <ending column>\n");
		return 1;
	} else {
		maze_file = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
		path_file = argv[4];
		start_row = atoi(argv[5]);
		start_col = atoi(argv[6]);
		goal_row = atoi(argv[7]);
		goal_col = atoi(argv[8]);
	}
	
	

	FILE *file;
	file = fopen(maze_file, "r");
	if(file == NULL){
		fprintf(stderr, "Cannot open file named %s\n", maze_file);
		return 1;
	}

	FILE *solve_file;
	solve_file = fopen(path_file, "w");
	if(solve_file == NULL){
		fprintf(stderr, "Cannot open file named %s\n", path_file);
		return 1;
	}

	struct maze_room maze[num_rows*num_cols];

	int r; int c;
	for(r=0; r<num_rows; r++){
		for(c=0; c<num_cols; c++){
			unsigned int hex;
			fscanf(file, "%1x", &hex);
			int i = get_index(r, c, num_cols);
			maze[i].row = r;
			maze[i].col = c;
			maze[i].visited = 0;
			create_room_connections(&maze[i], hex);
		}
	}

	
	
	
	dfs(start_row, start_col, goal_row, goal_col, num_rows,
		num_cols, maze, solve_file, 0);

	print_maze(maze, num_rows, num_cols);
	
	int cur_row = start_row;
	int cur_col = start_col;
	while(cur_row!=goal_row || cur_col != goal_col){
		printf("(%d, %d)\n", cur_row, cur_col);
		int next_cur_row = maze[get_index(cur_row, cur_col, num_cols)].next->row;
		int next_cur_col = maze[get_index(cur_row, cur_col, num_cols)].next->col;
		printf("index, %d\n", get_index(cur_row, cur_col, num_cols));
		printf("next_cur_row, %d\n", next_cur_row);
		cur_row = next_cur_row;
		cur_col = next_cur_col;
	}

	fclose(file);
	return 0;


}

