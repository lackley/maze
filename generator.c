#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "generator.h"

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
 * Given a pointer to the room and a Direction to travel in, return a pointer to the room
 * that neighbors the current room on the given Direction.
 * For example: get_neighbor(&maze[3][4], EAST) should return &maze[3][5]
 *
 * Parameters:
 *	- maze: the array that contains our maze_room structs
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- room: pointer to the current room
 *	- dir: Direction to get the neighbor from
 *
 * Returns:
 *	- pointer to the neighboring room
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
 * Given a direction, get its opposite
 *
 * Parameters:
 *	- dir: direction
 *
 * Returns:
 *	- the opposite direction to dir
 */
Direction get_opposite_dir(Direction dir) {
	
	if(dir == NORTH){
		return SOUTH;
	} else if(dir == EAST){
		return WEST;
	} else if(dir == SOUTH){
		return NORTH;
	} else {
		return EAST;
	}
}

/*
 * Given an array of the four directions, randomizes the order of the directions
 *
 * Parameters:
 *	- directions: an array of Direction enums of size 4
 *
 * Returns:
 *	- nothing - the array should be shuffled in place
 */
void shuffle_array(Direction directions[]) {
	
	
	int i;
	for(i=0; i<4; i++){
		int num = rand() % 4;
		//swap directions[i] with directions[num]
		//IS THIS RIGHT FOR SWAPPING
		Direction tmp;
		tmp = *(directions + i);
		*(directions + i) = *(directions + num);
		*(directions + num) = tmp;

	}

}

/*
 * Recursive function for stepping through a maze and setting its connections/walls using
 * the drunken_walk algorithm
 *
 * Parameters:
 *	- maze: an array of maze_room structs representing your maze
 * 	- row: row of the current room
 *	- col: column of the current room
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *
 * Returns:
 *	- nothing - the state of your maze should be saved in the maze array being passed
 *	  in (make sure to use pointers correctly!).
 */
void drunken_walk(struct maze_room maze[], int row, int col,
        int num_rows, int num_cols) {
	
	Direction dirs[4] = {NORTH, SOUTH, WEST, EAST};
	shuffle_array(dirs);

	int room_index = get_index(row, col, num_cols);
	maze[room_index].visited = 1;

	int i;
	for(i=0; i<4; i++){
		Direction dir = dirs[i];

		//if the neighboor is out of bounds store a wall
		if(neighbor_out_bound(num_rows, num_cols, &maze[room_index], dir)==1){
			maze[room_index].walls[dir] = 1; 
		} else {
			struct maze_room *neighbor = get_neighbor(maze,num_rows, num_cols, &maze[room_index], dir);
			//QUESTION: will neighboor->visited be automatically initialized to 0
			if(neighbor->visited == 0){
				maze[room_index].walls[dir] = 0;
				drunken_walk(maze, neighbor->row, neighbor->col, num_rows, num_cols);
			} else {
				Direction opposite = get_opposite_dir(dir);
				
				//if the neighboor has no connection in opposite dir, store wall
				if(neighbor->walls[opposite] == -1){
					maze[room_index].walls[dir] = 1;
				} else{
					maze[room_index].walls[dir] = neighbor->walls[opposite];
				}
			}
		}
	}

}

/*
 * Represents a maze_room struct as a decimal number that is 
   later to be converted to a hexadecimal number based on its connections
 *
 * Parameters:
 *	- room: a struct maze_room to be converted to a hexadecimal number
 *
 * Returns:
 *	- the hexadecimal representation of room
 */
int encode_room(struct maze_room room) {
	int dec = room.walls[3]*8 + room.walls[2]*4 + 
			  room.walls[1]*2 + room.walls[0];

	return dec;
}

/*
 * Represents a maze_room array as a hexadecimal array based on its connections
 *
 * Parameters:
 *	- maze: an array of maze_room structs representing the maze
 *	- num_rows: number of rows in the maze
 *	- num_cols: number of columns in the maze
 *	- nums: an array of hexadecimal numbers representing the maze
 *
 * Returns:
 *	- nothing - each maze_room in the maze should be represented
 *	  as a hexadecimal number  and put in nums at the corresponding index
 */
void encode_maze(struct maze_room maze[], int num_rows, int num_cols,
		int result[]) {
	int i;
	for(i=0; i<num_cols*num_rows;i++){
		result[i] = encode_room(maze[i]);
	}
}

/*
 * Main function
 *
 * Parameters:
 *	- argc: the number of command line arguments - for this function 4
 *	- **argv: a pointer to the first element in the command line
 *            arguments array - for this function:
 *		      ["generator", <output file>, <number of rows>, <number of columns>]
 *
 * Returns:
 *	- 0 if program exits correctly, 1 if there is an error
 */
int main(int argc, char **argv) {
	
	srand(time(NULL));

	char *file_name;
	int num_rows;
	int num_cols;

	if (argc != 4) {
		printf("Incorrect number of arguments.\n");
		printf("./generator <output file> <number of rows>");
		printf(" <number of columns>\n");
		return 1;
	} else {
		file_name = argv[1];
		num_rows = atoi(argv[2]);
		num_cols = atoi(argv[3]);
	}
	
	struct maze_room maze[num_rows*num_cols];
	int r; int c;
	for(r=0; r<num_rows;r++){
		for(c=0; c<num_cols; c++){
			int index = get_index(r, c, num_cols);
			maze[index].row = r;
			maze[index].col = c;
			maze[index].visited = 0;
			int i;
			for(i=0; i<4; i++){
				maze[index].walls[i] = -1;
			}
		}
	}

	drunken_walk(maze, 0, 0, num_rows, num_cols);

	int result[num_rows*num_cols];

	encode_maze(maze, num_rows, num_cols, result);

	FILE *file;
	file = fopen(file_name, "w");
	if(file == NULL){
		fprintf(stderr, "Cannot open file named %s\n", file_name);
		return 1;
	}

	int rr; int cc;
	for(rr=0; rr<num_rows; rr++){
		for(cc=0; cc<num_cols;cc++){
			int index = get_index(rr, cc, num_cols);
			fprintf(file, "%x", result[index]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
	return 0;

}

