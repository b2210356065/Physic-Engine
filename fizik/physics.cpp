//
// Created by Lenovo on 27.06.2024.
//
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include "shapes.h"

using namespace std;
class physics {

public:

    struct phsyicalvalues{
        double value;
        int shape[9][9];
        double space[2];
        double velocity[2];
        double acce[2];
        double coloumbvalue;
        int size;
        phsyicalvalues *next;
    };
    std::vector<std::vector<int>> space;
    double milisecond = 0.0001;
    double coloumbconst=0.899;
    double gravityx = 0;
    double graconts = 0.674;
    double gravityy = 0;

    phsyicalvalues *head;


    physics(): space(508, std::vector<int>(508, 0)) {
    }
    ~physics(){
        while (head!= nullptr){
            phsyicalvalues *s=head->next;
            delete head;
            head=s;
        }
    }
    int timepersecond(phsyicalvalues *head,int n){
        phsyicalvalues *temp=head;
        double max=0;
        double maxmass=0;
        double minmass=INT_MAX;
        while (temp!= nullptr){
            if(temp->value>maxmass){
                maxmass=temp->value;
            }
            if(temp->value<minmass){
                minmass=temp->value;
            }
            if(temp->velocity[0]>max){
                max=temp->velocity[0];
            }
            if(temp->velocity[1]>max){
                max=temp->velocity[1];
            }
            temp=temp->next;
        }
        milisecond/=(max);
        return n/milisecond;
    }
    void assignandrun(int n,vector<vector<int>> &simulation_values){
        objects(simulation_values);
        n= timepersecond(head,n);
        for (int i = 0; i < n; ++i) {
            move(head);
        }
    }
    void objects(vector<vector<int>>& sim_val) {
        phsyicalvalues* temp = nullptr;
        phsyicalvalues* prev = nullptr;

        for (int i = 0; i < sim_val.size(); ++i) {
            temp = new phsyicalvalues;
            if (i == 0) head = temp;
            if (prev != nullptr) prev->next = temp;

            temp->value = sim_val[i][0];
            for (int j = 0; j < 9; ++j) {
                for (int k = 0; k < 9; ++k) {
                    temp->shape[j][k] = shapes().all_shapes[sim_val[i][1]][j][k];
                }
            }
            temp->space[0] = sim_val[i][2];
            temp->space[1] = sim_val[i][3];
            temp->velocity[0] = sim_val[i][4];
            temp->velocity[1] = sim_val[i][5];
            temp->acce[0] = sim_val[i][6];
            temp->acce[1] = sim_val[i][7];
            temp->coloumbvalue = sim_val[i][8];
            temp->size = sim_val[i][9];

            prev = temp;
        }
        if (temp != nullptr) temp->next = nullptr;
    }


    void collision(int kase, phsyicalvalues*& node1, phsyicalvalues *& node2) {
        if(kase == 0){
            if(distance(node1,node2)<=node1->size+node2->size){
                collisionmechanics(node1,node2);
            }
        } else if(kase == 1){
            node1->velocity[1] = -1 * node1->velocity[1];
        } else if(kase == 2){
            node1->velocity[0] = -1 * node1->velocity[0];
        }
    }


    void collisionmechanics(phsyicalvalues*&node1, phsyicalvalues*&node2){
        double sum = node1->value + node2->value;
        if (sum != 0.0) {
            double v1 =
                    (node1->velocity[0] * (node1->value - node2->value) + 2 * node2->value * node2->velocity[0]) / sum;
            double v3 =
                    (node1->velocity[1] * (node1->value - node2->value) + 2 * node2->value * node2->velocity[1]) / sum;
            double v2 =
                    (node2->velocity[0] * (node2->value - node1->value) + 2 * node1->value * node1->velocity[0] )/ sum;
            double v4 =
                    (node2->velocity[1] * (node2->value - node1->value) + 2 * node1->value * node1->velocity[1] )/ sum;

            node1->velocity[0] = v1;
            node1->velocity[1] = v3;
            node2->velocity[0] = v2;
            node2->velocity[1] = v4;
        }

        double net_coulomb=(node1->coloumbvalue+node2->coloumbvalue)/(node1->value+node2->value);
        node1->coloumbvalue=net_coulomb*node1->value;
        node2->coloumbvalue=net_coulomb*node2->value;
    }

    void controlspace(phsyicalvalues *&node){
        if(node->space[1] < 4) {
            node->space[1] = 4+(4-node->space[1]);
            collision(1, node, node);
        } else if(node->space[1]>503) {
            node->space[1] = 503-(node->space[1]-503);
            collision(1, node, node);
        }
        if(node->space[0] < 4) {
            node->space[0] = 4+(4-node->space[0]);
            collision(2, node, node);
        } else if(node->space[0]>503){
            node->space[0] = 503-(node->space[0]-503);
            collision(2, node, node);
        }
    }

    void spacex( phsyicalvalues *&node) {
        node->space[1] += node->velocity[1] * milisecond;
        node->space[0] += node->velocity[0] * milisecond;
        controlspace(node);
        int intparty = static_cast<int>(node->space[1]);
        int intpartx = static_cast<int>(node->space[0]);
        space[intparty][intpartx]=node->value;
    }

    void force(phsyicalvalues* &node1, phsyicalvalues*& node2) {
        double y_to = node2->space[1];
        double x_to = node2->space[0];
        double y_from = node1->space[1];
        double x_from = node1->space[0];
        double r2 = (x_from - x_to) * (x_from - x_to) + (y_from - y_to) * (y_from - y_to);
        double coul=coloumbconst*node1->coloumbvalue*node2->coloumbvalue/r2;
        r2 = graconts * node2->value * node1->value / r2;
        double dx = r2 * (x_to - x_from)/ distance(node1,node2);
        double dy = r2 * (y_to - y_from)/distance(node1,node2);
        double ex=coul*(x_to-x_from)/distance(node1,node2);
        double ey=coul*(y_to-y_from)/distance(node1,node2);
        node1->velocity[1] +=gravityy+ milisecond * (dy + ey)/ node1->value;
        node1->velocity[0] +=gravityx+ milisecond * (dx + ex) / node1->value;
    }
    double distance(phsyicalvalues *&node1,phsyicalvalues *&node2){
        double y_to = node2->space[1];
        double x_to = node2->space[0];
        double y_from = node1->space[1];
        double x_from = node1->space[0];
        double r2 = sqrt((x_from - x_to) * (x_from - x_to) + (y_from - y_to) * (y_from - y_to));
        return r2;
    }

    void velocityx(phsyicalvalues *&node) {
        phsyicalvalues *current = head;
        while (current != nullptr) {
            if(current == node) {
                current = current->next;
                continue;
            }
            force(node, current);
            collision(0,node,current);
            current = current->next;
        }
        node->velocity[1] += node->acce[1] * milisecond;
        node->velocity[0] += node->acce[0] * milisecond;
    }


    void move(phsyicalvalues *&node) {
        phsyicalvalues *current = node;
        while (current != nullptr) {
            spacex(current);
            velocityx(current);
            current = current->next;
        }
    }
};