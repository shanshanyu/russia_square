#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

/*
 *打印一个方块的形状，用二维数组实现
 *
 *
 */


typedef struct tetris{
  int pos[4][4];
}tetris;

//定义外围边框的二维数组
int map[20][20];
//俄罗斯方块有 7 种图形，4种变形，一共 28 种图形，通过二维结构体数组来存储
tetris rock[7][4];

//图形位置
int x,y;
//图形以及变形
int s,r;

//存储得分
int score;

//运行标志
int run_flag;

void clear_rock();

//设置光标位置
void gotoxy(int x,int y){
  printf("\033[%d;%dH",y,x);
}


//初始化图形数组，初始化二维数组 i->y  j->x
void init_map(){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j= 0; j < 20; j++){
      if(i ==0 || i == 19 || j== 0 || j == 19)
        map[i][j] = 1;
      else
        map[i][j] = 0;
    }
  }
}

//显示图形
void print_map(){
  int i,j;
  for(i = 0; i < 20; i++){
    for(j = 0; j < 20; j++){
      if(map[i][j] == 1){
        gotoxy(2*j+1,i+1);
        printf("[]");
      }else{
        gotoxy(2*j+1,i+1);
        printf("  ");
      }     
    }
    printf("\n");
  }
  gotoxy(43,15);
  printf("得分:%d",score);
}


//显示方块,在指定位置显式指定图形  s 指定图形，r指定变形
void print_square(int s,int r,int x,int y,int flag){
  int i,j;
  if(flag == 1){
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(rock[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("[]");
        }
      printf("\n");
    }
  }else{
    for(i = 0; i < 4; i++){
      for(j = 0; j < 4; j++)
        if(rock[s][r].pos[i][j] == 1){
          gotoxy(2*(j+x)+1,i+1+y);
          printf("  ");
        }
      printf("\n");
    }
  }
}

void init_rock(){
  //第一种图形的 4 种变形
  rock[0][0].pos[0][1] = 1;
  rock[0][0].pos[1][0] = 1;
  rock[0][0].pos[1][1] = 1;
  rock[0][0].pos[1][2] = 1;

  rock[0][1].pos[0][0] = 1;
  rock[0][1].pos[1][0] = 1;
  rock[0][1].pos[1][1] = 1;
  rock[0][1].pos[2][0] = 1;

  rock[0][2].pos[0][0] = 1;
  rock[0][2].pos[0][1] = 1;
  rock[0][2].pos[0][2] = 1;
  rock[0][2].pos[1][1] = 1;

  rock[0][3].pos[0][1] = 1;
  rock[0][3].pos[1][0] = 1;
  rock[0][3].pos[1][1] = 1;
  rock[0][3].pos[2][1] = 1;

  //第二种图形的 4 种变形
  rock[1][0].pos[0][0] = 1;
  rock[1][0].pos[0][1] = 1;
  rock[1][0].pos[1][1] = 1;
  rock[1][0].pos[1][2] = 1;

  rock[1][1].pos[0][1] = 1;
  rock[1][1].pos[1][0] = 1;
  rock[1][1].pos[1][1] = 1;
  rock[1][1].pos[2][0] = 1;

  rock[1][2].pos[0][0] = 1;
  rock[1][2].pos[0][1] = 1;
  rock[1][2].pos[1][1] = 1;
  rock[1][2].pos[1][2] = 1;

  rock[1][3].pos[0][1] = 1;
  rock[1][3].pos[1][0] = 1;
  rock[1][3].pos[1][1] = 1;
  rock[1][3].pos[2][0] = 1;

  //第三种图形的 4 种变形
  rock[2][0].pos[0][0] = 1;
  rock[2][0].pos[1][0] = 1;
  rock[2][0].pos[2][0] = 1;
  rock[2][0].pos[2][1] = 1;

  rock[2][1].pos[0][0] = 1;
  rock[2][1].pos[0][1] = 1;
  rock[2][1].pos[0][2] = 1;
  rock[2][1].pos[1][0] = 1;

  rock[2][2].pos[0][0] = 1;
  rock[2][2].pos[0][1] = 1;
  rock[2][2].pos[1][1] = 1;
  rock[2][2].pos[2][1] = 1;

  rock[2][3].pos[0][2] = 1;
  rock[2][3].pos[1][0] = 1;
  rock[2][3].pos[1][1] = 1;
  rock[2][3].pos[1][2] = 1;


  //第四种图形的 4 种变形
  rock[3][0].pos[0][1] = 1;
  rock[3][0].pos[0][2] = 1;
  rock[3][0].pos[1][0] = 1;
  rock[3][0].pos[1][1] = 1;

  rock[3][1].pos[0][0] = 1;
  rock[3][1].pos[1][0] = 1;
  rock[3][1].pos[1][1] = 1;
  rock[3][1].pos[2][1] = 1;

  rock[3][2].pos[0][1] = 1;
  rock[3][2].pos[0][2] = 1;
  rock[3][2].pos[1][0] = 1;
  rock[3][2].pos[1][1] = 1;

  rock[3][3].pos[0][0] = 1;
  rock[3][3].pos[1][0] = 1;
  rock[3][3].pos[1][1] = 1;
  rock[3][3].pos[2][1] = 1;

  //第五种图形的 4 种变形
  rock[4][0].pos[0][1] = 1;
  rock[4][0].pos[1][1] = 1;
  rock[4][0].pos[2][0] = 1;
  rock[4][0].pos[2][1] = 1;

  rock[4][1].pos[0][0] = 1;
  rock[4][1].pos[1][0] = 1;
  rock[4][1].pos[1][1] = 1;
  rock[4][1].pos[1][2] = 1;

  rock[4][2].pos[0][0] = 1;
  rock[4][2].pos[0][1] = 1;
  rock[4][2].pos[1][0] = 1;
  rock[4][2].pos[2][0] = 1;

  rock[4][3].pos[0][0] = 1;
  rock[4][3].pos[0][1] = 1;
  rock[4][3].pos[0][2] = 1;
  rock[4][3].pos[1][2] = 1;

  //第六种图形的 4种变形
  rock[5][0].pos[0][0] = 1;
  rock[5][0].pos[1][0] = 1;
  rock[5][0].pos[2][0] = 1;
  rock[5][0].pos[3][0] = 1;

  rock[5][1].pos[0][0] = 1;
  rock[5][1].pos[0][1] = 1;
  rock[5][1].pos[0][2] = 1;
  rock[5][1].pos[0][3] = 1;

  rock[5][2].pos[0][0] = 1;
  rock[5][2].pos[1][0] = 1;
  rock[5][2].pos[2][0] = 1;
  rock[5][2].pos[3][0] = 1;

  rock[5][3].pos[0][0] = 1;
  rock[5][3].pos[0][1] = 1;
  rock[5][3].pos[0][2] = 1;
  rock[5][3].pos[0][3] = 1;

  //第七种图形的 4 种变形
  rock[6][0].pos[0][0] = 1;
  rock[6][0].pos[0][1] = 1;
  rock[6][0].pos[1][0] = 1;
  rock[6][0].pos[1][1] = 1;

  rock[6][1].pos[0][0] = 1;
  rock[6][1].pos[0][1] = 1;
  rock[6][1].pos[1][0] = 1;
  rock[6][1].pos[1][1] = 1;

  rock[6][2].pos[0][0] = 1;
  rock[6][2].pos[0][1] = 1;
  rock[6][2].pos[1][0] = 1;
  rock[6][2].pos[1][1] = 1;

  rock[6][3].pos[0][0] = 1;
  rock[6][3].pos[0][1] = 1;
  rock[6][3].pos[1][0] = 1;
  rock[6][3].pos[1][1] = 1;

}


//判断方块是否到达底部
int touch_buttom(int s,int r,int x,int y){
  int i,j;
  int flag = 0;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(rock[s][r].pos[i][j] == 1)
        if(map[y+i][x+j] == 1){
          flag = 1;
          break;
        }
  
  return flag;
}

//判断游戏是否结束，根据第一行是否有方块来判断
int is_dead(){
  int i;
  int flag = 0;
  for(i = 1; i < 19; i++)
    if(map[1][i] == 1)
      flag = 1;
  return flag;
}


//play_game负责一个图形的移动，移动到底部结束，play_game 也返回了
void play_game(pthread_mutex_t *mut){
  int i,j;
  s = rand() % 7;
  r = rand() % 4;
  x = 8;
  y = 1;
  
  //画出边框
  print_map();


  while(1){
    //方块再下落一次到达底部
    pthread_mutex_lock(mut);
    if(touch_buttom(s,r,x,y+1)){
      for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++){
          //把图形固定到map上去
          if(rock[s][r].pos[i][j] == 1){
            map[i+y][j+x] = 1;
          }
        }
      if(is_dead()){
        gotoxy(43,14);
        printf("game over");
        pthread_mutex_unlock(mut);
        run_flag = 0;
      }
      //方块到达底部后清除满的行，分数增加
      clear_rock();
      //函数退出前解锁
      pthread_mutex_unlock(mut);
      break;
    }else{
      //调整一个显示和不显示的逻辑，把sleep(1) 挪出去，给if判断加一把锁，锁不能加载这里，要加在if前面
      print_square(s,r,x,y,0);
      y++;
      print_square(s,r,x,y,1);
    }
    pthread_mutex_unlock(mut);
    sleep(1);
  } 

}

//通过键盘输入 'a' 's' 'w' 'd' 改变图形的位置
void *update_rock(void *arg){
  int ch;
  
  while(run_flag){
    ch = getchar();
    switch(ch){
      //旋转图形，顺时针旋转
      case 'w':
        if(!touch_buttom(s,(r+1)%4,x,y)){
          print_square(s,r,x,y,0);
          r = (r+1)%4;
          print_square(s,r,x,y,1);
        }
        break;
      case 'a':
        if(!touch_buttom(s,r,x-1,y)){
          print_square(s,r,x,y,0);
          x--;
          print_square(s,r,x,y,1);
        }
        break;
      case 's':
        pthread_mutex_lock(arg);
        if(!touch_buttom(s,r,x,y+1)){
          print_square(s,r,x,y,0);
          y++;
          print_square(s,r,x,y,1);
        }
        pthread_mutex_unlock(arg);
        break;
      case 'd':
        if(!touch_buttom(s,r,x+1,y)){
          print_square(s,r,x,y,0);
          x++;
          print_square(s,r,x,y,1);
        }
        break;
    }

  }
    return NULL;
}

//如果一行满了，把这一行消掉，上方的行向下移动
void clear_rock(){
  int i,j;
  int k,m;
  int sum;
  for(i = 18; i > 0; i--){
    sum = 0;
    for(j = 1; j < 19; j++)
      if(map[i][j] == 1){
        sum++;
      }

    if(sum == 18){
      //上面的行下移一行
      for(k = i; k > 0; k--)
        for(m = 1; m < 19; m++){
          if(k == 1)
            map[k][m] = 0;
          else
            map[k][m] = map[k-1][m];
        }
      //下移后应该继续判断刚才那一行
      i++;
      score++;
    }
  } 
  //消除方块后重新绘图
  print_map(); 
}



int main(){
  struct termios term,save_term;
  pthread_t tid;
  pthread_mutex_t mut;
  int err;
  
  //初始化map 和 rock
  init_map();
  init_rock();
  //清屏
  printf("\033[2J]");
  //隐藏光标
  printf("\033[?25l");
  
  //获取、设置终端属性
  if(tcgetattr(STDIN_FILENO,&term) < 0){
    perror("tcgetattr()");
    exit(1);
  }
  save_term = term;

  //关闭回显和规范模式
  term.c_lflag &= ~(ECHO|ICANON); 
  term.c_cc[VTIME] = 0;
  term.c_cc[VMIN] = 1;

  if(tcsetattr(STDIN_FILENO,TCSANOW,&term) < 0){
    perror("tcsetattr()");
    exit(1);
  }
  
  //互斥量初始化
  err = pthread_mutex_init(&mut,NULL);
  if(err){
    fprintf(stderr,"pthread_mutex_init():%s\n",strerror(err));
    exit(1);
  }

  //创建一个线程来处理键盘按键
  err = pthread_create(&tid,NULL,update_rock,&mut);
  if(err){
    fprintf(stderr,"pthread_create():%s\n",strerror(err));
    exit(1);
  }

  run_flag = 1;
  while(run_flag){
    play_game(&mut);
  }

  pthread_join(tid,NULL);
  pthread_mutex_destroy(&mut);

  //恢复终端属性
  if(tcsetattr(STDIN_FILENO,TCSANOW,&save_term) < 0){
    perror("tcsetattr()");
    exit(1);
  }


  printf("terminate normally");
  exit(0);
}
