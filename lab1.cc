#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <map>  //needed this so did C++ instead of C
#include <ctime>
#include <chrono>
using namespace std;
/*
  Brian Parks
  CSE 5441 Lab 1 Dr. Jones
  9 Feb 2016
*/
int num_boxes = 0;
int m_width = 0;
int m_height = 0;


/*
  Struct to hold box information
*/

typedef struct BoxInfo{
  int xPos,yPos,h,w;
  int n_topNghbrs; //number of top neighbors
  int n_botNghbrs; //number of bottom neighbors
  int n_lftNghbrs; //number of left neighbors
  int n_rhtNghbrs; //number of right neighbors
  float dsv;
  float updtd_dsv; //temportally hold updated value
  int *t_Nghbrs; //top neighbors
  int *b_Nghbrs; //bottom neighbors
  int *l_Nghbrs; //left neighbors
  int *r_Nghbrs; //right neighbors
} Box;

//returns contact length between box b and n, see implemnation for more detail
int get_neighbor_contact_length(Box b, Box n, int side_or_not);

//returns 1 iff convergance condition is met, see implemnation for more detail
int is_converged(float epsln,int num_boxes, map<int,Box> Box_Map);

int main(int argc, char* argv[]){

if(argc < 2){
  perror("Incorrect number of parameters. Please see example below\n");
  perror("a.out <affect_rate><epsilon> < datafile");
  exit(1);
}
/*
 Use a map to with <Key,Value> = <Box_id,Box>
*/
map<int,Box> Box_Map;
//char *inFileName = argv[3];
float epsilon;
float affect_rate;


sscanf(argv[2],"%f",&epsilon);
//printf("epsilon is %f\n",epsilon);

sscanf(argv[1],"%f",&affect_rate);
//printf("affect_rate is %f\n",affect_rate);

char line[256];  //256 bytes should be enough for one line from input file
/*FILE *inFilePointer;
inFilePointer = fopen(inFileName,"r");
if(inFilePointer == NULL){
    perror("error opening data input file\n");
    exit(2);
}*/
/*
printf("line is %s\n",line);
std::cin.getline(line,256);
printf("line is %s\n",line);
std::cin.getline(line,256);
printf("line is %s\n",line);
std::cin.getline(line,256);
printf("line is %s\n",line);
std::cin.getline(line,256);
printf("line is %s\n",line);
std::cin.getline(line,256);
printf("here x\n");*/
std::cin.getline(line,256);
if(false){
  perror("error reading file, or file contains no information\n");
  exit(3);
}
const char delim[3] = " \t";
char *token;

token = strtok(line,delim);
//printf("token is %s\n",token);
num_boxes = atoi(token); //set number of boxes
//printf("number of boxes is: %d\n",num_boxes);

token = strtok(NULL,delim);
//printf("token is %s\n",token);
m_width = atoi(token); //set width of grid
//printf("b width is: %d\n",m_width);

token = strtok(NULL,delim);
//printf("token is %s\n",token);
m_height = atoi(token); //set height of grid
std::cin.getline(line,256);
//std::cin.getline(line,256);
while(!cin.eof()){ //gets box id line
  //printf("here\n");
  //printf("line[0] is %c xxxxx\n",line[0]);
  while(line[0] == '\n'){
    std::cin.getline(line,256); //skip if line is blank
  }
  //printf("line is %s\n",line);
  Box b;
   token = strtok(line,delim);
   while(token == NULL){
       std::cin.getline(line,256);
       token = strtok(line,delim);

   }

  //printf("here3\n");
  //printf("token is %s\n",token);
  //if(strncmp(token,"\n",3)){continue;}
  int box_id = atoi(token);

  if(box_id == -1){break;} //if token is -1, exit loop
  //printf("box id is: %d\n",box_id);
//printf("here2\n");
  std::cin.getline(line,256); // box pos and H and W

  token = strtok(line,delim);
  int upr_lft_y = atoi(token);
  //printf("uppr lft y is %d\n",upr_lft_y);

  token = strtok(NULL,delim);
  int upr_lft_x = atoi(token);
  //printf("uppr lft x is %d\n",upr_lft_x);

  token = strtok(NULL,delim);
  int box_height = atoi(token);
  //printf("box height is %d\n",box_height);

  token = strtok(NULL,delim);
  int box_width = atoi(token);
  //printf("box width is %d\n",box_width);

  Box_Map[box_id].xPos = upr_lft_x;
  Box_Map[box_id].yPos = upr_lft_y;
  Box_Map[box_id].h = box_height;
  Box_Map[box_id].w = box_width;

  std::cin.getline(line,256); // top neighbor line

  token = strtok(line,delim);
  int num_of_top_nghbrs = atoi(token);
  Box_Map[box_id].n_topNghbrs = num_of_top_nghbrs;
  //printf("num top neighbors: %d\n",num_of_top_nghbrs);

  if(num_of_top_nghbrs != 0){
    int *t_neighbors = (int *)malloc(sizeof(int)*num_of_top_nghbrs);
    int i;
  //  printf("Neighbors: ");

    for(i = 0; i < num_of_top_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
      //printf("(%d) %d \n",i,neighbor);
      t_neighbors[i] = neighbor;
    }

   Box_Map[box_id].t_Nghbrs = t_neighbors;
    std::cin.getline(line,256); //bottom neighbor line
  }else{
    std::cin.getline(line,256); // bottom neighbor line
  }




   // bottom neighbor line

  token = strtok(line,delim);
  int num_of_btm_nghbrs = atoi(token);
  Box_Map[box_id].n_botNghbrs = num_of_btm_nghbrs;
  //printf("num btm neighbors: %d\n",num_of_btm_nghbrs);
  if(num_of_btm_nghbrs != 0){
    int *b_neighbors = (int *)malloc(sizeof(int)*num_of_btm_nghbrs);
    int i;
  //  printf("Neighbors: ");
    for(i = 0; i < num_of_btm_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
      //printf("(%d) %d \n",i,neighbor);
      b_neighbors[i] = neighbor;
    }
    Box_Map[box_id].b_Nghbrs = b_neighbors;
    std::cin.getline(line,256); // left neighbor line
  }else{
    std::cin.getline(line,256); // left neighbor line
  }




  token = strtok(line,delim);
  int num_of_lft_nghbrs = atoi(token);
  Box_Map[box_id].n_lftNghbrs = num_of_lft_nghbrs;
  //printf("num lft neighbors: %d\n",num_of_lft_nghbrs);
  if(num_of_lft_nghbrs != 0){
    int *l_neighbors = (int *)malloc(sizeof(int)*num_of_lft_nghbrs);
    int i;
    //printf("Neighbors: ");
    for(i = 0; i < num_of_lft_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
      //printf("(%d) %d \n",i,neighbor);
      l_neighbors[i] = neighbor;

    }
    Box_Map[box_id].l_Nghbrs = l_neighbors;
    std::cin.getline(line,256); // right neighbor line
  }else{
    std::cin.getline(line,256); // right neighbor line
  }



  token = strtok(line,delim);
  int num_of_rght_nghbrs = atoi(token);
  Box_Map[box_id].n_rhtNghbrs = num_of_rght_nghbrs;
  //printf("num right neighbors: %d\n",Box_Map[box_id].n_rhtNghbrs);
  if(num_of_rght_nghbrs != 0){
    int *r_neighbors = (int *)malloc(sizeof(int)*num_of_rght_nghbrs);
    int i;
    //printf("Neighbors: ");
    for(i = 0; i < num_of_rght_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
    //x  printf("(%d) %d \n",i,neighbor);
      r_neighbors[i] = neighbor;

    }
    Box_Map[box_id].r_Nghbrs = r_neighbors;
  }else{

  }

  std::cin.getline(line,256); //dsv
  token = strtok(line,delim);
  float dsv;
  sscanf(token,"%f",&dsv);
  //printf("dsv is %f\n",dsv);
  Box_Map[box_id].dsv = dsv;
  std::cin.getline(line,256);
}
struct timeval tv1,tv2;
gettimeofday(&tv1,NULL);
clock_t begin = clock();

chrono::system_clock::time_point t1 = chrono::system_clock::now();
int p = 0;
while(is_converged(epsilon,num_boxes,Box_Map) != 1 /*&& p < 40000*/){
  p++;
  //printf("here\n");
  int i;
  for(i = 0; i < num_boxes; i++){
     Box b = Box_Map[i];
     float box_new_dsv;
     float average_surrounding_temp;

     //top neighbors
     float top_temp_sum = 0;
     int j;
     for(j = 0; j < b.n_topNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].t_Nghbrs[j]];
       float t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,0);
       top_temp_sum += (t * len);
     }

     //bottom neighbors
     float btm_temp_sum = 0;

     for(j = 0; j < b.n_botNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].b_Nghbrs[j]];
       float t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,0);
       btm_temp_sum += (t * len);
     }

     //Left neighbors
     float lft_temp_sum = 0;

     for(j = 0; j < b.n_lftNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].l_Nghbrs[j]];
       float t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,1);
       lft_temp_sum += (t * len);
     }


     //right neighbors
     float rght_temp_sum = 0;

     for(j = 0; j < b.n_rhtNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].r_Nghbrs[j]];
       float t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,1);
       rght_temp_sum += (t * len);
     }
     int perim = 2*(b.w + b.h);

     //set outside boxes to current box dsv
     if(top_temp_sum == 0){
       top_temp_sum = b.dsv * b.w;
     }
     if(btm_temp_sum == 0){
       btm_temp_sum = b.dsv * b.w;
     }
     if(lft_temp_sum == 0){
       lft_temp_sum = b.dsv * b.h;
     }
     if(rght_temp_sum == 0){
       rght_temp_sum = b.dsv * b.h;
     }


     average_surrounding_temp = (top_temp_sum + btm_temp_sum + rght_temp_sum
       + lft_temp_sum) / (float)(perim);
     Box_Map[i].updtd_dsv = b.dsv - (b.dsv - average_surrounding_temp) * affect_rate;

  }
  int k;
  for(k = 0; k < num_boxes; k++){

    Box_Map[k].dsv = Box_Map[k].updtd_dsv;
    //printf("I: %d Box %d:  dsv %f\n",p,k,Box_Map[k].dsv);
  }
  //printf("----------------------------------------------\n");
} //end loop;
chrono::system_clock::time_point t2 = chrono::system_clock::now();
chrono::duration<double> time_span = chrono::duration_cast< chrono::duration<double> > (t2 - t1);
gettimeofday(&tv2,NULL);
clock_t end = clock();
float clck_secs = float(end - begin) / CLOCKS_PER_SEC;

float max_dsv = Box_Map[0].dsv;
float min_dsv = Box_Map[0].dsv;
int i;
for(i = 0; i < num_boxes; i++){
   Box b = Box_Map[i];
   if(b.dsv < min_dsv){
     min_dsv = b.dsv;
   }
   if(b.dsv > max_dsv){
     max_dsv = b.dsv;
   }
}

printf("\n\n***********************************************************\n");
printf("Dissipation converged in %d iterations.\n",p);
printf("Max DSV     : %f\n",max_dsv);
printf("Min DSV     : %f\n",min_dsv);
printf("Affect rate : %f\n",affect_rate);
printf("Epsilon     : %f\n",epsilon);
printf("Elapsed time (clock) in seconds  : %f\n",clck_secs);
printf ("Elapsed time (time) in seconds   : %f \n",
         (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
         (double) (tv2.tv_sec - tv1.tv_sec));
std::cout << "Elapsed time (chrono) in seconds : " << time_span.count() << "\n\n";
printf("***********************************************************\n");
/*std::cout << "finished computation at " << std::ctime(&end_time)
             << "elapsed time: " << elapsed_seconds.count() << "s\n";*/


/*for(i = 0; i < num_boxes; i++){
 printf("Box: %d DSV: %f \n",i,Box_Map[i].dsv);

}*/

for(i = 0; i < num_boxes; i++){
  free(Box_Map[i].t_Nghbrs);
  free(Box_Map[i].b_Nghbrs);
  free(Box_Map[i].l_Nghbrs);
  free(Box_Map[i].r_Nghbrs);

}
return 0;
}
/*
  @returns: 1 iff converged condition is met.
  Converge condition:
    MAX(Boxes.dsv) - MIN(Boxes.dsv) >= MAX(Boxes.dsv) * epsilon
  @params: epsln user defined Epsilon
           n_boxes total number of boxes
           Box_Map: a map containing <Box_id,Box> pairs


*/
int is_converged(float epsln, int n_boxes, map<int,Box> Box_Map){

  float max_dsv = Box_Map[0].dsv;
  float min_dsv = Box_Map[0].dsv;
  int i;
  for(i = 0; i < n_boxes; i++){

     Box b = Box_Map[i];

     if(b.dsv < min_dsv){
       min_dsv = b.dsv;
     }
     if(b.dsv > max_dsv){
       max_dsv = b.dsv;
     }
  }

  float diff = (max_dsv - min_dsv);

  int ret_val = 0;


  if(diff <= epsln*max_dsv ){
    ret_val = 1;
  }
  return ret_val;
}
/*
  Calculates contact distance between two boxes.
  @params Box b: box you are computing
          Box n: box b's neighbor
          side_or_not: 1 iff side (so left or right box, otherwise top or bottom)
  @returns contact length between box b and box n
*/

int get_neighbor_contact_length(Box b, Box n, int side_or_not){
  int bx = b.xPos;
  int by = b.yPos;
  int bh = b.h;
  int bw = b.w;

  int nx = n.xPos;
  int ny = n.yPos;
  int nw = n.w;
  int nh = n.h;
/*
  printf("box b x %d w %d \n",b.xPos,b.w);
  printf("box n x %d w %d \n",n.xPos,n.w);
  printf("box b y %d h %d \n",b.yPos,b.h);
  printf("box n y %d h %d \n",n.yPos,n.h);
*/
  int len;

  if(side_or_not == 1){
    if(ny < by){
      len = (nh + ny) - by;
      if( (by+bh) < (ny + nh) ){
        len = len - ( (ny + nh) - (by + bh) );
      }
    }else{
      if( (ny + nh) < (by + bh)){
        len = nh;
      }else{
        len =(by+bh) - ny;
      }
    }
  }else{
    if(nx < bx){
      len = (nw + nx) - bx;
      if( (bx+bw) < (nx + nw) ){
        len = len - ( (nx + nw) - (bx + bw) );
      }
    }else{
      if( (nx + nw) < (bx + bw)){
        len = nw;

      }else{

        len =(bx+bw) - nx;
      }
    }
  }

 return len;
}


/**************   TESTING SECTION FOR REFERENCE *****************/
/*

printf("\n\n");
printf("###########################\n");
printf("##    Struct Testing     ##\n");
printf("###########################\n");
int i;

for(i = 0; i < num_boxes; i++){
  printf("Box ID: %d\n",i);
  printf("Box xPos : %d\n",Box_Map[i].xPos);
  printf("Box yPos : %d\n",Box_Map[i].yPos);
  printf("Box h    : %d\n",Box_Map[i].h);
  printf("Box w    : %d\n",Box_Map[i].w);

  //top test
  printf("Box T_N  : %d\n",Box_Map[i].n_topNghbrs);
  printf("Box T_N_L: ");
  int j;
  for(j = 0; j < Box_Map[i].n_topNghbrs; j++){
    printf(" %d",Box_Map[i].t_Nghbrs[j]);
  }
  printf("\n");

  //bottom test
  printf("Box B_N  : %d\n",Box_Map[i].n_botNghbrs);
  printf("Box B_N_L: ");

  for(j = 0; j < Box_Map[i].n_botNghbrs; j++){
    printf(" %d",Box_Map[i].b_Nghbrs[j]);
  }
  printf("\n");

  //left test
  printf("Box L_N  : %d\n",Box_Map[i].n_lftNghbrs);
  printf("Box L_N_L: ");

  for(j = 0; j < Box_Map[i].n_lftNghbrs; j++){
    printf(" %d",Box_Map[i].l_Nghbrs[j]);
  }
  printf("\n");

  //right test
  printf("Box R_N  : %d\n",Box_Map[i].n_rhtNghbrs);
  printf("Box R_N_L: ");

  for(j = 0; j < Box_Map[i].n_rhtNghbrs; j++){
    printf(" %d",Box_Map[i].r_Nghbrs[j]);
  }
  printf("\n");
  printf("Box DSV  : %f\n",Box_Map[i].dsv);
  printf("##########################\n");
} //end print t */
/*
int test = get_neighbor_contact_length(Box_Map[38],Box_Map[22],0);
printf("length was %d\n",test);
test = get_neighbor_contact_length(Box_Map[36],Box_Map[20],0);
printf("36 20 length was %d\n",test);
test = get_neighbor_contact_length(Box_Map[36],Box_Map[35],1);
printf("36 35 length was %d\n",test);
test = get_neighbor_contact_length(Box_Map[36],Box_Map[37],1);
printf("36 37 length was %d\n",test);*/
//printf("entering is converged loop;\n");

/*
//bottom neighbor tests
int test = get_neighbor_contact_length(Box_Map[1],Box_Map[5],0);
printf("box 1 neighbor 5 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[1],Box_Map[6],0);
printf("box 1 neighbor 6 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[1],Box_Map[7],0);
printf("box 1 neighbor 7 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[1],Box_Map[8],0);
printf("box 1 neighbor 8 length was %d\n",test);
//top nieghtbor test
test = get_neighbor_contact_length(Box_Map[5],Box_Map[1],0);
printf("box 5 neighbor 1 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[6],Box_Map[1],0);
printf("box 6 neighbor 1 length was %d\n",test);
//printf("Box 7 x: %d y: %d nT: %d neig: %d\n",Box_Map[7].xPos,Box_Map[7].yPos, Box_Map[7].n_topNghbrs, Box_Map[7].t_Nghbrs[0]);
 test = get_neighbor_contact_length(Box_Map[7],Box_Map[1],0);
printf("box 7 neighbor 1 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[8],Box_Map[1],0);
printf("box 8 neighbor 1 length was %d\n",test);
//left nieghbor test
test = get_neighbor_contact_length(Box_Map[8],Box_Map[7],1);
printf("box 8 neighbor 7 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[8],Box_Map[9],1);
printf("box 8 neighbor 9 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[8],Box_Map[10],1);
printf("box 8 neighbor 10 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[8],Box_Map[11],1);
printf("box 8 neighbor 11 length was %d\n",test);
//right neighbor tests
test = get_neighbor_contact_length(Box_Map[6],Box_Map[7],1);
printf("box 6 neighbor 7 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[6],Box_Map[9],1);
printf("box 6 neighbor 9 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[6],Box_Map[10],1);
printf("box 6 neighbor 10 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[6],Box_Map[11],1);
printf("box 6 neighbor 11 length was %d\n",test);

test = get_neighbor_contact_length(Box_Map[7],Box_Map[9],0);
printf("box 7 neighbor 9 length was %d\n",test);

test = get_neighbor_contact_length(Box_Map[12],Box_Map[5],0);
printf("box 12 neighbor 5 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[5],Box_Map[12],0);
printf("box 5 neighbor 12 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[12],Box_Map[6],0);
printf("box 12 neighbor 6 length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[6],Box_Map[12],0);
printf("box 6 neighbor 12 length was %d\n",test);
test = get_neighbor_contact_length(Box_Map[13],Box_Map[14],1);
printf("box 13 neighbor 14 length was %d\n",test);
test = get_neighbor_contact_length(Box_Map[14],Box_Map[13],1);
printf("box 14 neighbor 13 length was %d\n",test);*/
