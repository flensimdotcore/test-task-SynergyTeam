#include <iostream>
#include <conio.h>
#include <ctime>
#include <string>
//#include <thread>

using namespace std;

class Elevator { // Создаём экземпляр класса лифт
    public:
        int cur_floor, max_floor, dest_floor, max_clicked_floor;
        int qC[100] = {};
        int qF[100] = {};
        bool stop;
        int direction;

    Elevator(int aCur_floor, int aMax_floor){
        cur_floor = aCur_floor; // Этаж, на котором в данный момент находится лифт (проезжает этаж, либо стоит на нём)
        max_floor = aMax_floor; // Максимальный в доме этаж
        dest_floor = cur_floor; // dest_floor - этаж назначения
        stop = true;
        direction = 0;
        max_clicked_floor = 0;
    }

    void addToCQueue(int dest_floor){ // Добавление нажатого в лифте этажа в массив нажатий из лифта
        qC[dest_floor] = 1;
    }

    void addToFQueue(int dest_floor){ // Добавление нажатия на этаже в массив нажатий на этажах
        qF[dest_floor] = 1;
    }

    void dltFromCQueue(int dltd_floor){ // Удаление нажатого в лифте этажа из массив нажатий из лифта
        qC[dltd_floor] = 0;
    }

    void dltFromFQueue(int dltd_floor){ // Удаление нажатия на этаже из массива нажатий на этажах
        qF[dltd_floor] = 0;
    }

    void Move(){

        /* Определение направления движения и этажа назначения
           direction = 1 - движение вверх
           direction = 0 - стоянка (отсутствие задач)
           direction = -1 - движение вниз
        */

        if (direction == 1){
            for(int i = cur_floor;i <= max_floor;i++){
                if(qC[i] == 1){
                    dest_floor = i;
                    stop = false;
                    direction = 1;
                    return;
                }
                if (qF[i] == 1 && i > max_clicked_floor){
                    max_clicked_floor = i;
                }
                dest_floor = max_clicked_floor;
            }
        }
        else if (direction == -1 ){
            max_clicked_floor = 0;
            for(int i = cur_floor;i >= 1;i--){
                if(qC[i] == 1 || qF[i] == 1){
                    dest_floor = i;
                    stop = false;
                    direction = -1;
                    return;
                }
            }
        }
        else if (direction == 0){
            for(int i = max_floor;i >= 1;i--){
                if(qC[i] == 1 || qF[i] == 1){
                    dest_floor = i;
                    stop = false;
                    if(dest_floor < cur_floor)
                        direction = -1;
                    if(dest_floor > cur_floor)
                        direction = 1;
                    return;
                }
                else{
                    direction = 0;
                }
            }
        }
    }

    void MoveTo(){

        // Движение лифта к назначенному этажу

            if (cur_floor < dest_floor)
            {
                cur_floor+=1;
            }
            else if (cur_floor > dest_floor)
            {
                cur_floor-=1;
            }
            else{
                stop = true;
                dltFromCQueue(cur_floor);
                dltFromFQueue(cur_floor);
                direction = 0;
            }
    }

    void ShowInfo(){

        // Вывод информации о движении лифта

        cout << "Cabin on floor " << cur_floor;
        if (cur_floor < dest_floor)
            {
                cout << ". Going up. " << "<Press any key to enter new command>\n";
            }
            else if (cur_floor > dest_floor)
            {
                cout << ". Going down. " << "<Press any key to enter new command>\n";
            }
            else{
                cout << ". Stopped. Doors opened\n";
            }
    }


    void getInput()
    {

        // Считывание клавиатуры и добавление эдементов в массивы нажатий

        string ch;
        if (kbhit())
        {
            getline(cin, ch);

            // Вызовы на этажах

            if (ch[0] == 70){
                string numF; numF[0] = ch[1]; numF[1] = ch[2];
                int floorF = stoi(numF);
                if (floorF > max_floor){
                    cout << "Only type numbers in interval [1," << max_floor << "]\n";
                    return;
                }
                addToFQueue(floorF);
                stop = false;
            }

            // Вызовы в лифте

            if (ch[0] == 67){
                string numC; numC[0] = ch[1]; numC[1] = ch[2];
                int floorC = stoi(numC);
                if (floorC > max_floor){
                    cout << "Only type numbers in interval [1," << max_floor << "]\n";
                    return;
                }
                if ((floorC < cur_floor) && direction == -1){
                    addToCQueue(floorC);
                    stop = false;
                }
                else if ((floorC > cur_floor) && direction == 1){
                    addToCQueue(floorC);
                    stop = false;
                }
                else if (direction == 0){
                    addToCQueue(floorC);
                    stop = false;
                }

            }
        }
    }

};

int main()
{
    int m_f, i_f;
    cout << "Welcome to Elevator Emulator!\n\n" << "Input max floor in interval [1, 99]: ";
    cin >> m_f;
    if (m_f > 99){
        cout << "You typed number greater than 99\n" << "Max floor set to 99\n";
        m_f = 99;
    }
    cout << "\nInput initial floor: ";
    cin >> i_f;
    if (i_f > m_f){
        cout << "You typed number greater than number of floors\n" << "Initial floor set to max floor\n";
        i_f = m_f;
    }
    cout << "\nEnter new command:\nFxx - elevator call on floor xx\nCxx - elevator call in cabin to floor xx\n";

    Elevator elev(i_f, m_f); // Создание объекта класса лифт
    int t = -1000;

    while(true)
    {
        elev.getInput();
        elev.Move();
        if (!elev.stop && (clock() - t > 1000)){
            t = clock();
            elev.ShowInfo();
            elev.MoveTo();
        }

    }
    return 0;
}
