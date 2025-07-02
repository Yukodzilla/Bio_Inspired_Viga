#include "head.h"

// Restricciones y funciones del problema a optimizar
// double Restricciones(vector V){//O(1)
//     double penalizacion = 0.0;
//     penalizacion+=Constraint_G1(V);
//     penalizacion+=Constraint_G2(V);
//     penalizacion+=Constraint_G2(V);
//     penalizacion+=Constraint_G4(V);
//     penalizacion+=Constraint_G5(V);
//     penalizacion+=Constraint_G6(V);
//     penalizacion+=Constraint_G7(V);
//     return penalizacion;
// }
double Restricciones(vector V){//O(1)
    double penalizacion = 0.0,costo=1e+10;
    if(Constraint_G1(V)>0.0) penalizacion+=costo;
    if(Constraint_G2(V)>0.0) penalizacion+=costo;
    if(Constraint_G3(V)>0.0) penalizacion+=costo;
    if(Constraint_G4(V)>0.0) penalizacion+=costo;
    if(Constraint_G5(V)>0.0) penalizacion+=costo;
    if(Constraint_G6(V)>0.0) penalizacion+=costo;
    if(Constraint_G7(V)>0.0) penalizacion+=costo;
    return penalizacion;
}
double Constraint_G1(vector V){//O(1)
    double tao_X=function_Tao(V);
    if(tao_X>Tao_max) return ((tao_X-Tao_max)*(tao_X-Tao_max));
    return 0.0;
}
double Constraint_G2(vector V){//O(1)
    double sigma_X=function_Sigma(V);
    if(sigma_X>Sigma_max) return ((sigma_X-Sigma_max)*(sigma_X-Sigma_max));
    return 0.0;
}
double Constraint_G3(vector V){//O(1)
    if(V.x_1>V.x_4) return ((V.x_1-V.x_4)*(V.x_1-V.x_4));
    return 0.0;
}
double Constraint_G4(vector V){//O(1)
    //No es un polinomio, pero me gusta llamarlo asi
    double polinomio=(0.10471*(V.x_1*V.x_1)) + (0.04811*(V.x_3*V.x_4)*(14.0+V.x_2)) - 5.0;
    if(polinomio>0.0) return (polinomio*polinomio);
    return 0.0;
}
double Constraint_G5(vector V){//O(1)
    if(0.125>V.x_1) return ((0.125-V.x_1)*(0.125-V.x_1));
    return 0.0;
}
double Constraint_G6(vector V){//O(1)
    double delta_X=function_Delta(V);
    if(delta_X>Delta_max) return ((delta_X-Delta_max)*(delta_X-Delta_max));
    return 0.0;
}
double Constraint_G7(vector V){//O(1)
    double Pc_X=function_Pc(V);
    if(P_Carga>Pc_X) return ((P_Carga-Pc_X)*(P_Carga-Pc_X));
    return 0.0;
}
int Constraint_Xn(double x,double limite_inf,double limite_sup){//O(1)
    if(x<limite_inf || x>limite_sup) return -1;
    return 0;
}
int verificaLimites(vector V){//O(1)
    if(Constraint_Xn(V.x_1,0.1, 2.0)==-1) return -1;
    if(Constraint_Xn(V.x_2,0.1,10.0)==-1) return -1;
    if(Constraint_Xn(V.x_3,0.1,10.0)==-1) return -1;
    if(Constraint_Xn(V.x_4,0.1, 2.0)==-1) return -1;
    if(V.x_4<V.x_1) return -1;
    return 0;
}
double function_Tao(vector V){//O(1)
    double t_prima,t_biprima,R,x2,raiz;
    t_prima = function_Tao_1(V);
    t_biprima = function_Tao_2(V);
    R=function_R(V);
    x2=V.x_2;
    raiz=(t_biprima*t_biprima)+(t_prima*t_biprima*x2/R)+(t_prima*t_prima);//Aqui modifique ya que (2.0*t_prima*t_biprima*x2/(2.0*R))
    if(raiz<0){
        printf("\nRaiz negativa en function_Tao");
        return 0.0;
    }
    return sqrt(raiz);
}
double function_Tao_1(vector V){//O(1)
    if(V.x_1==0.0 || V.x_2==0.0){
        printf("\nError x_1 y x_2 no pueden ser 0 en function_Tao_1");
        return 0.0;
    }
    return P_Carga/(sqrt(2)*V.x_1*V.x_2);
}
double function_Tao_2(vector V){//O(1)
    double J=function_J(V);
    if(J==0.0){
        printf("\nError J no pueden ser 0 en function_Tao_2");
        return 0.0;
    }
    return (function_M(V)*function_R(V))/J;
}
double function_J(vector V){//O(1)
    double J = 2*(sqrt(2)*V.x_1*V.x_2*( ((V.x_2*V.x_2)/12.0) + (((V.x_1+V.x_3)*(V.x_1+V.x_3))/4.0) ));
    return J;
}
double function_R(vector V){//O(1)
    double R = ((V.x_2*V.x_2)/4.0) + (((V.x_1+V.x_3)*(V.x_1+V.x_3))/4.0);
    return sqrt(R);
}
double function_M(vector V){//O(1)
    double M = P_Carga*(L + (V.x_2/2.0));
    return M;
}
double function_Sigma(vector V){//O(1)
    double Sigma=(6.0*P_Carga*L);
    if(V.x_3==0.0 || V.x_4==0.0){
        printf("\nError x_3 y x_4 no pueden ser 0 en function_Sigma");
        return 0.0;
    }
    return Sigma/=(V.x_4*(V.x_3*V.x_3));
}
double function_Delta(vector V){//O(1)
    double Sigma=(4.0*P_Carga*(L*L*L));
    if(V.x_3==0.0 || V.x_4==0.0){
        printf("\nError x_3 y x_4 no pueden ser 0 en function_Delta");
        return -1.0;
    }
    return Sigma/=(E*(V.x_3*V.x_3*V.x_3)*V.x_4);
}
double function_Pc(vector V){//O(1)
    double Pc,x3,x4;
    x3=V.x_3;
    x4=V.x_4;
    if(L==0.0 || G==0.0){
        printf("\nError L y G no pueden ser 0 en function_Pc");
        return -1.0;
    }
    if(E<0.0 || G<0.0){
        printf("\nError E y G no pueden ser negativos en function_Pc");
        return -1.0;
    }
    if(x3<0.0) x3=(-1.0)*x3;
    if(x4<0.0) x4=(-1.0)*x4;
    if(tipo_PC==1){
        Pc=((4.013*E*x3*(x4*x4*x4))/(6.0*(L*L)))*(1.0-((V.x_3/(2.0*L))*sqrt(E/(4.0*G))));
        // Pc=102372.449*(1.0-(0.02823462197*V.x_3))*x3*(x4*x4*x4);
    }
    else{
        Pc=64746.022*(1.0-((V.x_3/(2.0*L))*sqrt(E/(4.0*G))))*x3*(x4*x4*x4);
        // Pc=64746.022*(1.0-(0.028234*V.x_3))*x3*(x4*x4*x4);
    }
    return Pc;
}
