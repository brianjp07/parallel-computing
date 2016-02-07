#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/time.h>
#include <map>  //needed this so did C++ instead of C
#include <ctime>
using namespace std;
/*
  Brian Parks
  CSE 5441 Lab 1 Dr. Jones
  9 Feb 2016
*/
int num_boxes = 0;
int m_width = 0;
int m_height = 0;




typedef struct BoxInfo{
  int xPos,yPos,h,w;
  int n_topNghbrs;
  int n_botNghbrs;
  int n_lftNghbrs;
  int n_rhtNghbrs;
  double dsv;
  double updtd_dsv; //temportally hold updated value
  int *t_Nghbrs;
  int *b_Nghbrs;
  int *l_Nghbrs;
  int *r_Nghbrs;
} Box;

int get_neighbor_contact_length(Box b, Box n, int side_or_not);
int is_converged(double epsln,int num_boxes, map<int,Box> Box_Map);

int main(int argc, char* argv[]){

if(argc < 3){
  perror("Incorrect number of parameters. Please see example below\n");
  perror("a.out <file> <epsilon> <affect_rate>");
  exit(1);
}
map<int,Box> Box_Map;
char *inFileName = argv[1];
double epsilon;
double affect_rate;


sscanf(argv[2],"%lf",&epsilon);
//printf("epsilon is %lf\n",epsilon);

sscanf(argv[3],"%lf",&affect_rate);
//printf("affect_rate is %lf\n",affect_rate);
char line[256];
FILE *inFilePointer;
inFilePointer = fopen(inFileName,"r");

if(inFilePointer == NULL){
    perror("error opening data input file\n");
    exit(2);
}

if(fgets(line,256,inFilePointer) == NULL){
  perror("error reading file, or file contains no information\n");
  exit(3);
}
const char delim[3] = " \t";
char *token;

token = strtok(line,delim);
num_boxes = atoi(token);
//printf("number of boxes is: %d\n",num_boxes);

token = strtok(NULL,delim);
m_width = atoi(token);
//printf("b width is: %d\n",m_width);

token = strtok(NULL,delim);
m_height = atoi(token);
//printf("b height is: %d\n",m_height);


//fgets(line,256,inFilePointer); //skip blank

while(fgets(line,256,inFilePointer) != NULL){ //gets box id line
 fgets(line,256,inFilePointer); //skip blank
  Box b;
  token = strtok(line,delim);
  //if(strncmp(token,"\n",3)){continue;}
  int box_id = atoi(token);

  if(box_id == -1){break;}
  //printf("box id is: %d\n",box_id);

  fgets(line,256,inFilePointer); // box pos and H and W

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

  fgets(line,256,inFilePointer); // top neighbor line

  token = strtok(line,delim);
  int num_of_top_nghbrs = atoi(token);
  Box_Map[box_id].n_topNghbrs = num_of_top_nghbrs;
  //printf("num top neighbors: %d\n",num_of_top_nghbrs);

  if(num_of_top_nghbrs != 0){
    int *t_neighbors = (int *)malloc(sizeof(int)*num_of_top_nghbrs);
    int i;
    //printf("Neighbors: ");

    for(i = 0; i < num_of_top_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
      //printf("(%d) %d \n",i,neighbor);
      t_neighbors[i] = neighbor;
    }

   Box_Map[box_id].t_Nghbrs = t_neighbors;
    fgets(line,256,inFilePointer); //bottom neighbore line
  }else{
    fgets(line,256,inFilePointer); // bottom neighbor line
  }




   // bottom neighbor line

  token = strtok(line,delim);
  int num_of_btm_nghbrs = atoi(token);
  Box_Map[box_id].n_botNghbrs = num_of_btm_nghbrs;
  //printf("num btm neighbors: %d\n",num_of_btm_nghbrs);
  if(num_of_btm_nghbrs != 0){
    int *b_neighbors = (int *)malloc(sizeof(int)*num_of_btm_nghbrs);
    int i;
    //printf("Neighbors: ");
    for(i = 0; i < num_of_btm_nghbrs; i++){
      token = strtok(NULL,delim);
      int neighbor = atoi(token);
      //printf("(%d) %d \n",i,neighbor);
      b_neighbors[i] = neighbor;
    }
    Box_Map[box_id].b_Nghbrs = b_neighbors;
    fgets(line,256,inFilePointer); // left neighbor line
  }else{
    fgets(line,256,inFilePointer); // left neighbor line
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
    fgets(line,256,inFilePointer); // right neighbor line
  }else{
    fgets(line,256,inFilePointer); // right neighbor line
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
      //printf("(%d) %d \n",i,neighbor);
      r_neighbors[i] = neighbor;

    }
    Box_Map[box_id].r_Nghbrs = r_neighbors;
  }else{

  }

  fgets(line,256,inFilePointer); //dsv
  token = strtok(line,delim);
  double dsv;
  sscanf(token,"%lf",&dsv);
  //printf("dsv is %lf\n",dsv);
  Box_Map[box_id].dsv = dsv;
}
int test = get_neighbor_contact_length(Box_Map[4],Box_Map[1],0);
printf("length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[4],Box_Map[7],0);
printf("length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[4],Box_Map[3],1);
printf("length was %d\n",test);
 test = get_neighbor_contact_length(Box_Map[4],Box_Map[5],1);
printf("length was %d\n",test);
clock_t begin = clock();
//std::chrono::time_point<std::chrono::system_clock> start, end;
//start = std::chrono::system_clock::now();
int p;
while(is_converged(epsilon,num_boxes,Box_Map) != 1 && p < 300){
  p++;
  //printf("here\n");
  int i;
  for(i = 0; i < num_boxes; i++){
     Box b = Box_Map[i];
     double box_new_dsv;
     double average_surrounding_temp;

     double top_temp_sum = 0;

     int j;
     for(j = 0; j < b.n_topNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].t_Nghbrs[j]];
       double t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,0);
       top_temp_sum = t * len;
     }


     double btm_temp_sum = 0;


     for(j = 0; j < b.n_botNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].b_Nghbrs[j]];
       double t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,0);
       btm_temp_sum = t * len;
     }

     //Left
     double lft_temp_sum = 0;

     for(j = 0; j < b.n_lftNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].l_Nghbrs[j]];
       double t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,1);
       lft_temp_sum = t * len;
     }


     //right
     //printf("Right\n");
     double rght_temp_sum = 0;

     for(j = 0; j < b.n_rhtNghbrs; j++){
       Box t_n_box = Box_Map[Box_Map[i].r_Nghbrs[j]];
       double t = t_n_box.dsv;
       int len = get_neighbor_contact_length(b,t_n_box,1);
       rght_temp_sum = t * len;
     }
     int perim = 2*b.w + 2*b.h;
     /*if(b.xPos == 0 and b.yPos == 0){
       perim = perim - (b.w + b.h);
     }else if(b.xPos == 0){
       perim = perim - b.h;
     }else if(b.yPos == 0){
       perim = perim - b.w;
     }*/

     average_surrounding_temp = (top_temp_sum + btm_temp_sum + rght_temp_sum
       + lft_temp_sum) / (double)(perim);
     Box_Map[i].updtd_dsv = b.dsv - (b.dsv - average_surrounding_temp) * affect_rate;
     printf("Box: %d ast is: %lf\n",i,average_surrounding_temp);
     printf("Box: %d updated_dsv is: %lf\n\n\n",i,Box_Map[i].updtd_dsv);

  /*  if(is_converged(epsilon,num_boxes,Box_Map) == 1){
      printf("converged\n");
      break;
    }*/
  }
  int k;
  for(k = 0; k < num_boxes; k++){
    //printf("box %d updated\n",k);

    //printf("old dsv %lf\n",Box_Map[k].dsv);
    //printf("updated dsv %lf\n",Box_Map[k].updtd_dsv);
    Box_Map[k].dsv = Box_Map[k].updtd_dsv;
    printf("I: %d Box %d:  dsv %lf\n",p,k,Box_Map[k].dsv);
  }
  //printf("----------------------------------------------\n");
} //end loop;
  //std::time_t end_time = std::chrono::system_clock::to_time_t(end);
clock_t end = clock();
double clck_secs = double(end - begin) / CLOCKS_PER_SEC;
  double max_dsv = Box_Map[0].dsv;
  double min_dsv = Box_Map[0].dsv;
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
printf("***********************************************************\n");
printf("Dissipation converged in %d iterations.\n",p);
printf("Max DSV     : %lf\n",max_dsv);
printf("Min DSV     : %lf\n",min_dsv);
printf("Affect rate : %lf\n",affect_rate);
printf("Epsilon     : %lf\n",epsilon);
printf("Elapsed time (time) in seconds: %lf\n",clck_secs);

/*std::cout << "finished computation at " << std::ctime(&end_time)
             << "elapsed time: " << elapsed_seconds.count() << "s\n";*/
for(i = 0; i < num_boxes; i++){
  free(Box_Map[i].t_Nghbrs);
  free(Box_Map[i].b_Nghbrs);
  free(Box_Map[i].l_Nghbrs);
  free(Box_Map[i].r_Nghbrs);

}
return 0;
}

int is_converged(double epsln, int n_boxes, map<int,Box> Box_Map){

  double max_dsv = Box_Map[0].dsv;
  double min_dsv = Box_Map[0].dsv;
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
  double diff = (max_dsv - min_dsv) / max_dsv;
  //printf("max dsv is: %lf\n",max_dsv);
//  printf("min dsv is: %lf\n",min_dsv);
  int ret_val = 0;
  //double epMax = diff;
//  printf("diff %lf  epMax  %lf\n",diff,epMax);
  if(diff <= epsln){
    ret_val = 1;
  }
  return ret_val;
}


int get_neighbor_contact_length(Box b, Box n, int side_or_not){
  int bx = b.xPos;
  int by = b.yPos;
  int bh = b.h;
  int bw = b.w;

  int nx = n.xPos;
  int ny = n.yPos;
  //printf("box y %d h %d \n",b.yPos,b.h);
  //printf("box y %d h %d \n",n.yPos,n.h);
  int nh = n.h;
  int nw = n.w;
  int by_tlt_space = (by+bh);
  //printf("by_tlt_space is %d\n",by_tlt_space);
  int bx_tlt_space = (bx+bw);
  int ny_tlt_space = (ny+nh);
  //printf("ny_tlt_space is %d\n",ny_tlt_space);
  int nx_tlt_space = (nx+nw);

  int yDiff = abs(ny_tlt_space - by_tlt_space);
  //printf("ydiff is %d\n",yDiff);
  int xDiff = abs(nx_tlt_space - bx_tlt_space);

  int len = 0;
  if(side_or_not == 1){ //it is a side length we are computing
    if(yDiff != 0){
      if(by_tlt_space <= ny_tlt_space){
        len = ny_tlt_space - yDiff;
      }else{
        len = by_tlt_space - yDiff;
      }

    }else{
      len = b.h;
    }
  }else{ //it is top or bottom length we are computing
    if(xDiff != 0){
      if(bx_tlt_space <= nx_tlt_space){
        len = nx_tlt_space - xDiff;
      }else{
        len = bx_tlt_space - xDiff;
      }
    }else{
      len = b.w;
    }
  }

 return len;
}

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
  printf("Box DSV  : %lf\n",Box_Map[i].dsv);
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
