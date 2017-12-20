#ifndef  CABASE.H
#define  CABASE.H

class CAbase{
        /*
         * Base Class for Cellular Automaton
         */
    public:
        //all member functions are declared inside class declaration so they will be executed inline

        //declaring the constructor with direct initialization
        CAbase(int input_rows, int input_columns){
            //: rows(input_rows), columns(input_columns), now(lifes) //member init list
                    rows = input_rows;
                    columns = input_columns;
                    size = rows*columns;
                    now = new int[size];
                    future = new int[size];
                    golMode = true;
                    snakeMode = false;
                    xposHead = 0;
                    dirHead = 0;
                    yposHead = 0;
                    lenSnake = 0;
                    dead = false;
                    growth = false;
        };
        //declaring the destructor
        ~CAbase(){
            delete now;
            delete future;
        };

        //GETTERS
        //declaring member function to access private data members
        int getRows() {
            return rows;
        }
        int getColumns() {
            return columns;
        }
        int* getNow() {
            return now;
        }
        int getCell(int row, int column) {
            return now[((row-1)*columns)+column-1];
        }

        // GETTERS to manipulate Snake Data Member
        int getXpos(){
            return xposHead;
        }
        int getYpos(){
            return yposHead;
        }
        int getDirection(){
            return dirHead;
        }
        int getLength(){
            return lenSnake;
        }
         void setAlive(){
            dead = false;
        }

        // GETTERS to ask for current game mode
        bool getGolMode(){
            return golMode;
        }
        bool getSnakeMode(){
            return snakeMode;
        }

        //GETTERS for preventing snake running backwards

        bool getLeftallowed(){
            if (getCell(xposHead-1,yposHead)==lenSnake-1){return false;}
            else{return true;}
        }

        bool getRightallowed(){
            if (getCell(xposHead+1,yposHead)==lenSnake-1){return false;}
            else{return true;}
        }
        bool getUpallowed(){
            if (getCell(xposHead,yposHead-1)==lenSnake-1){return false;}
            else{return true;}
        }
        bool getDownallowed(){
            if (getCell(xposHead,yposHead+1)==lenSnake-1){return false;}
            else{return true;}
        }
        // GETTER for SNAKE status:
        bool getIsAlive(){
            if (dead == false){return true;}
            else{return false;}
        }
        // GETTER for growth
        bool getGrowth(){
            return growth;
        }

        //SETTERS
        //declaring member functions to set private data members
        void setRows(int new_rows) {
            rows = new_rows;
        }
        void setColumns(int new_columns) {
            columns = new_columns;
        }
        void setCell(int row, int column, int new_value, int* array) {
            array[((row-1)*columns)+column-1] = new_value;
        }
        //sets a new value in Array:
        void setLazyNow(int index, int new_value) {
            now[index] = new_value;
        }
        void resetSize(int new_rows, int new_columns){
            setColumns(new_columns);
            setRows(new_rows);
            delete[] now;
            delete[] future;
            now = new int[rows * columns];
            future = new int [rows * columns];
            int i;
            for(i=0;i<(rows*columns);i++){
                now[i]=0;
                future[i]=0;
            }
        }
        // setter for growth
        void setGrowth(bool new_growth){
            growth = new_growth;
        }

        // SETTERS to switch the game mode
        void setGol(){
            golMode = true;
            snakeMode = false;
        }

        void setSnake(){
            golMode = false;
            snakeMode = true;
        }

        // SETTER to manipulate Snake Data Member
        void setXpos( int new_value){
            xposHead = new_value;
        }
        void setYpos( int new_value){
            yposHead = new_value;
        }
        void setDirection( int new_value){
            dirHead = new_value;
        }
        void setLength( int new_value){
            lenSnake = new_value;
        }

        //declaring member functions to calculate the next generation
        //count how many neighbours a cell(i,j) has
        // getNeighbours takes row in [1,rows] and column in [1,columns]
        int getNeighbours(int row, int column){
            int moduloincplusplusisabitchr = 0;
            int moduloincplusplusisabitchc = 0;
            int neighbourcount = 0;
            for( int i = (row-1); i < (row+2); i++){
                for( int j = (column-1); j < (column+2); j++){
                    // add all the neighbours
                    moduloincplusplusisabitchr = i-1;
                    moduloincplusplusisabitchc = j-1;
                    // if it is negative because c++ modulo sucks
                    if( moduloincplusplusisabitchr < 0){
                        moduloincplusplusisabitchr += rows;
                    }
                    // if it is negative because c++ modulo sucks
                    if(moduloincplusplusisabitchc < 0){
                        moduloincplusplusisabitchc += columns;
                    }
                    neighbourcount += getCell((moduloincplusplusisabitchr%rows)+1, ((moduloincplusplusisabitchc)%columns)+1);
                }
            }
            //substracting the cell itself, if it is alive
            if(getCell(row, column)==1){
                neighbourcount -= 1;
            }
            return neighbourcount;
        }

        //apply the GOL rules to build future generation
        void decideFuture(){
            if (golMode){
            for( int i = 1; i <= rows; i++){
                for( int j = 1; j <= columns; j++){
                    int neighbours;
                    neighbours = getNeighbours(i, j);
                    switch(neighbours) {

                        case 2:
                            //if cell is alive now, will stay alive
                            if(getCell(i,j)==1){
                                setCell(i, j, 1, future);
                            }
                            break;

                        case 3:
                            //if cell is alive now, will stay alive
                            //if cell was dead, will be reborn
                            setCell(i, j, 1, future);
                            break;

                        default:
                            //all other cases: cell dies or stays dead
                            setCell(i, j, 0, future);
                        }
                    }
                }
            } else{
                evolveSnake();
            }
        }

        // copy future to now
        void timeTravel(){
            for( int i = 0; i < (rows*columns); i++){
                    now[i] = future[i];
            }
        }
        
        void evolveSnake(){ //Body & Tail aufgabe 3
            bool growth = false; //as long as snake doesn't eat
            for (int i=0; i<size;i++){
                future[i]=now[i];        //copy all now values to future
            }

            switch (dirHead) {
            case 2:
                if (yposHead == columns){dead = true; break;} //BORDER
                if (getCell(xposHead, yposHead+1) >0){
                    dead=true;
                    break;
                } // Snake dies if it runs into itself
                if (getCell(xposHead, yposHead+1) <0){
                    lenSnake += 1;
                    growth = true;
                    setGrowth(true);
                    // If Snake moves on a field with food (-1), it grows
                    setCell( xposHead, yposHead+1, lenSnake, future);
                    setYpos(yposHead+1);
                    break;
                }
                setCell( xposHead, yposHead+1, lenSnake+1, future); //DOWN
                setYpos(yposHead+1);
                break;
            case 4:
                if (xposHead == 1){
                    dead = true;
                    break;
                }// BORDER
                if (getCell(xposHead-1, yposHead) >0){
                    dead=true;
                    break;
                }
                if (getCell(xposHead-1, yposHead) <0){
                    lenSnake += 1;
                    growth = true;
                    setGrowth(true);
                    setCell( xposHead-1, yposHead, lenSnake, future); //LEFT eat
                    setXpos(xposHead-1);
                    break;
                }
                    setCell( xposHead-1, yposHead, lenSnake+1, future); //LEFT
                    setXpos(xposHead-1);
                    break;
            case 6:
                if (xposHead == rows){
                    dead = true; break;
                } //BORDER
                if (getCell(xposHead+1, yposHead) >0){
                    dead=true; break;
                }
                if (getCell(xposHead+1, yposHead) <0){
                    lenSnake += 1;
                    growth = true;
                    setGrowth(true);
                    setCell( xposHead+1, yposHead, lenSnake, future); //RIGHT eat
                    setXpos(xposHead+1);
                    break;
                }
                    setCell( xposHead+1, yposHead, lenSnake+1, future);//RIGHT
                    setXpos(xposHead+1);
                    break;
            case 8:
                 if (yposHead == 1){
                     dead = true;
                     break;
                 } //BORDER
                 if (getCell(xposHead, yposHead-1) >0){
                     dead=true;
                     break;
                 }
                 if (getCell(xposHead, yposHead-1) <0){
                     lenSnake += 1;
                     growth = true;
                     setGrowth(true);
                     setCell( xposHead, yposHead-1, lenSnake, future); // UP eat
                     setYpos(yposHead-1);
                     break;
                 }
                    setCell( xposHead, yposHead-1, lenSnake+1, future); // UP
                    setYpos(yposHead-1);
                    break;
            }
            // aktualisiert snake in future (überall wo != 0 eins abziehen solange kein rand erreicht)
            for (int i=0; i<size;i++){
                if (future[i] != 0){
                    if (dead == false){
                        if (growth == false){
                            int y = future[i];
                            future[i] =y-1;}

                    }
                }
            }
        }

        //variable for testing
        int input = 4;
        //to change the direction
        void changeDirection(int input){
            int dirHead=input;
        }

    // declaring the private data members
    private:
        int rows;
        int columns;
        int size;
        int* now; //declaring array of data type dynamic integer array
        int* future; //declaring array of data type dynamic integer array
        int xposHead; //storing xposition of head
        int dirHead; //storing direction of head
        int yposHead; //storing yposition of head
        int lenSnake; //total length of Snake
        bool golMode; //tracks what game is played
        bool snakeMode; //tracks what game is played
        bool dead;   //stores condition snake is in (also active in gol, but not relevant there)
        bool growth; // snake grows --> food is gone --> new food has to be created
    };

#endif // CABASE.H
