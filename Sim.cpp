    #include "Sim.h"
    #include "math.h"
    #include "Atomo.h"
    #include "Celda.h"

    Sim::Sim()
    {

    }

    // Esto es un destructor que libera memoria
    Sim::~Sim()
    {

    }


    int Sim::prin()
    {
        cout << endl;
        cout << "Sim::prin " << endl;
        Datos();
        EscribirDatos();
        IniciarAtomos();

        if (cuad  == "cuad"){
            Cuadrada();
        }
        //EscribirDatos();
        //EscribirPosVel();
        if(metodo == "orig") {
            Aceleraciones();
        }
        if(metodo == "vel") {
            calcularVC();
            AceleracionesVC();
        }
        if(metodo == "cel") {
            IniciarCel();
            CalcularCel();
            AceleracionesCel();
        }
        Aceleraciones();
        Simulacion();

        // Inicializa el vector de posiciones


        return 0;
    }

    void Sim::Datos()
    {
        cout << endl;
        cout << "Sim::Datos" << endl;

        temp = 1.0;
        dens = 0.65;


        na = 6; // Numero de atomos
        nd = 3; // Numero de dimensiones
        nc = 1000; // Numero de configuraciones

        cuad = "cuad";
        metodo = "cel"; // Indicamos el meto do  de la optimization.

        dt = 0.001; // Delta de tiempo
        d= 1.0;
        L = pow((na/ dens),1.0/nd);
        r = d / 2.0;
        rc = 3.0;
        v0 = 1.5 * sqrt(temp * nd/2.0);
        drc = 0.5;
        rcd = rc + drc;
        nrc = (rc + 0.4);
        // Indicamos cada cuantos pasos vamos a actualizar la lista de vecinos.
        ncvc = 10;

        ncp =10;// Numero de configuraciones parciales
        nci = nc / ncp;

    }


    void Sim :: EscribirDatos() {

        cout << endl;
        cout << "Sim::EscribirDatos " << endl;

        // Escribir datos iniciales
        cout << endl;
        cout << "Datos iniciales" << endl;
        cout << "na,nd,nc " << na << " "
                            << nd << " "
                            << nc << endl;

        cout << "temp,dens " << temp << " "
                            << dens << endl;

        cout << "dt,L,d,r " << dt << " "
                        << L << " "
                        << d << " "
                            << r << endl;

        cout << "cuad " << cuad << endl;

        cout << "v0, rc" << v0 << " "
                    << rc << endl;
        cout << "ncp,nci " << ncp << " " << nci << endl;

    }

    void Sim::IniciarAtomos() {
        cout << endl;
        cout << "Sim::IniciarAtomos " << endl;

        double pv[3]; // Vector de 3 dimensiones que funciona como auxiliar

        // Definir los objetos atomos
        for(int ia=0; ia < na; ia ++ ) {
            Atomo *att = new Atomo;
            atomos.push_back(att);
            atomos[ia]-> a.push_back(0.0);
            //atomos[ia] -> vc
        }
        int an = atomos.size();
        cout << "an " << an << endl;

        // Iniciar posiciones y velocidades al azar.
        for(int ia = 0; ia < na; ia ++ ) {
            for(int id = 0; id < nd; id++ ) {
                pv[id] = L * ( rand() % 10000 / 10000.);
                // atomos[ia] -> p[id] = pv[id];
                atomos[ia] -> p.push_back(pv[id]);
                // atomos[ia]-> a.push_back(0.0);
            }
        }

        for(int ia = 0; ia < na; ia ++ ) {
            for(int id = 0; id < nd; id++ ) {
                double r = rand() % 10000 / 10000.;
                r = (r * 2.0) - 1;
                pv[id] = v0 * r;
                // atomos[ia] -> v[id] = pv[id];
                atomos[ia] -> v.push_back(pv[id]);
            }
        }
    }

    void Sim::EscribirPosVel() {
        cout << endl;
        cout << "Sim::EscribirPosVel " << endl;

        //Escribir posiciones y velocidades iniciales

        cout << endl;
        cout << "Posiciones y velocidades iniciales" << endl;
        for(int ia=0; ia < na; ia ++ ) {
            cout << "ia " << ia << " ";
            for(int id = 0; id < nd; id ++ ) {
                double pi = atomos[ia] -> p[id];
                cout << pi << " ";
            }
            for (int id = 0; id < nd; id++ ) {
                double vi = atomos[ia] -> v[id];
                cout << vi << " ";
            }
            cout << endl;
        }
    }

    void Sim ::Simulacion() {
        cout << endl;
        cout << "Sim::Simulacion " << endl;

        double pp, dttt, ti, tf;
        ti= clock();

        // SImulacion
        // Ciclo de configuraciones
        for (int ic = 0; ic < nc; ic++) {

            if(ic % ncvc==0) {
                calcularVC();
            }

            // Ciclo de los atomos
            for(int ia = 0; ia < na; ia ++) {
                // Ciclo de las dimensiones
                for(int id = 0; id < nd; id ++ ) {
                    pp = atomos[ia] -> p[id] +
                        dt * atomos[ia] -> v[id] + dt * dt * atomos[ia] -> a[id] * 0.5;

                    if(pp > L) pp = pp - L;
                    if(pp < 0) pp = pp + L;
                    atomos[ia] -> p[id] = pp; // Se desplazan los atomos
                }
                //Ciclo de las dimensiones
            }
            //Ciclo de los atomos

            //Calcular la mitad de las velocidades
            for (int ia=0; ia< na; ia++){
                    for (int id=0; id< nd; id++){
                        atomos[ia] -> v[id]=
                        atomos[ia] -> v[id] +
                        dt * atomos[ia] -> a[id]/2.0;
                    }

            }

            // Aceleraciones
            if(metodo == "vc") {
                AceleracionesVC();
            }
            if(metodo == "orig") {
                Aceleraciones();
            }
            if(metodo=="cel") {
                AceleracionesCel();
            }

            // Calcular la segunda mitad de las aceleraciones
            for (int ia=0; ia< na; ia++){
                    for (int id=0; id< nd; id++){
                        atomos[ia] -> v[id]=
                            atomos[ia] -> v[id] + dt *
                                atomos[ia] -> a[id]/2.0;
                    }


            }

            if(ic==0){
                cout << " temp, dens, et, ec, ei, dttt " <<endl;
            }



            if(ic % nci == 0) {
                Prop();
                tf= clock();
                dttt= (tf- ti) / 1000.;
                cout << temp << " "<< dens << " "<< et << " "<< ec << " "<< ei << " "<< dttt << " " << " ";
                cout << ic << " " << endl;
            }
        }
        //Ciclo de las configuraciones
    }

    void Sim::Aceleraciones() {
    //    cout << endl;
    //    cout << "Sim::Aceleraciones " << endl;

        for (int ia = 0; ia < na; ia++) {
            for (int id = 0; id < nd; id++) {
                atomos[ia] -> a[id] = 0.0;
            }
        }

        double u=0;
        for (int ia = 0; ia < na - 1; ia++) {
            for (int ja = ia+1; ja < na; ja++) {
                double r = Dist(ia,ja); // Se calculo la distancia
                u = u + LJ(r);

            for(int id = 0; id < nd; id++) {
                atomos[ia] -> a[id] =
                    atomos[ia] -> a[id] + f * dis[id] / r ; // Cuidado aquí con la f
                atomos[ja] -> a[id] =
                    atomos[ja] -> a[id]- f * dis[id] / r ;
            }
            }
        }
    }

    double Sim::LJ(double r) {
        //cout << endl;
        //cout << "Sim::LJ " << endl;
       // double ri = 1.0 / r;
        //double r3 = ri * ri * ri;
        //double r6 = r3*r3;

        double u = 4.0 *
            (pow((1.0/r),12 ) -
                pow((1.0 / r),6));

        f = 24.0 *
            (2.0 * pow((1.0/r),12 )-
                pow((1.0 / r),6))/r;

        return (u);
    }

    double Sim:: Dist (int i, int j) {
    //    cout << endl;
    //    cout << "Sim::Dist " << endl;
        double r=0;

        for(int id = 0; id < nd; id++) {
            dis[id] = atomos[i] -> p[id] -
                atomos[j] -> p[id];
            // Condiciones periodicas
            if(fabs(dis [id]) > L / 2) {
                dis[id] = dis[id] -
                    fabs(dis[id]) / dis[id] * L;
            }

           r = r + dis[id] * dis[id];
        }
        r = sqrt(r);

        return (r);
    }

// Hay un error aqui:
    void Sim:: Cuadrada() {
        cout << endl;
        cout << "Sim:: Cuadrada "<< endl;

        int nl= L;
        double dl= L/nl;
        double x, y, z;
        int ia = 0;

        for(int iz=0; iz< nl; iz++){
            z = iz * dl;

            for(int iy=0; iy< nl; iy++){
                y = iy * dl;
                for(int ix=0; ix< nl; ix++){
                    x = ix * dl;
                    atomos[ia]-> p[0]=x;
                    atomos[ia]-> p[1]=y;
                    atomos[ia]-> p[2]=z;
                    ia++;
                }
            }

            if (nd==2) break;
        }
    }

    void Sim:: Prop() {
    //    cout << endl;
    //    cout << "Sim:: Prop "<< endl;

        //Energia cinetica
        et=ec=ei=0;
        for(int ia = 0 ; ia<na; ia++){
            for (int id=0 ; id< nd; id++){
                double vi = atomos [ia] -> v[id];

                ec=ec + vi * vi * 0.5;
            }
        }
            ec = ec/na;

            //Energia interna

            for(int ia = 0; ia< na-1; ia++){
                for(int ib = ia+1; ib< na; ib++){
                    double r= Dist(ia, ib);

                    ei = ei + LJ(r);

                }
            }
            ei = ei / na;
            et = ec + ei;
            temp = ec * 2.0 / nd ;
        }

    void Sim::calcularVC() {
        //cout << endl;
        //cout << "Sim::calcularVC" << endl;

        for (int ia =0; ia < na; ia++) {
            atomos[ia] -> vc.clear();
            atomos[ia] -> nv = 0;
        }

        for (int ia = 0; ia < na - 1; ia++) {
            for (int ja = ia+1; ja < na-1; ja++) {
                double r = Dist(ia,ja); // Se calculo la distancia
                // Estoy debía ir en las velocidades tambien
                if(r < rcd) {
                    atomos[ia]  -> vc.push_back(ja);
                    atomos[ja]  -> vc.push_back(ia);
                }
            }
            atomos[ia] -> nv = atomos[ia] -> vc.size();
        }
        atomos[na-1] -> nv = atomos[na-1] -> vc.size();
        //cout << "atomos[0] -> nv" << atomos[0] -> nv << endl;
    }


    void Sim::AceleracionesVC() {
        //    cout << endl;
        //    cout << "Sim::Aceleraciones " << endl;

        for (int ia = 0; ia < na; ia++) {
            for (int id = 0; id < nd; id++) {
                atomos[ia] -> a[id] = 0.0;
            }
        }

        double u=0;
        for (int ia = 0; ia < na; ia++) {
            int nv = atomos[ia] -> nv;
            for (int ja = 0; ja < nv; ja++) {
                int ja1 = atomos[ia] -> vc[ja];
                double r = Dist(ia,ja1); // Se calculo la distancia
                if(r < rc) {
                    u = u + LJ(r);
                    for(int id = 0; id < nd; id++) {
                        atomos[ia] -> a[id] =
                            atomos[ia] -> a[id] + f * dis[id] *0.5 ; // Cuidado aquí con la f
                        //atomos[ja] -> a[id] =
                            //atomos[ja] -> a[id]- f * dis[id] / r ;
                    }
                }

            }
        }
    }

void Sim::IniciarCel() {
    cout << "Inicializando celdas " << endl;

        int ncL = L;// Numero de celdas por lado que va a tener el cuadro
        int nct = ncL * ncL; //Numero total de celdas
        if(nd == 3) nct = nct * ncL;

        // Ciclo para crear las celdas
        for (int i =0; i < nct; i++) {
            // Creamos un objeto de la clase celda
            Celda *c = new Celda;
            // Guardamos el c en el vector
            celdas.push_back(c);
        }
        // Este es un dato global, se usa para las celdas
        nctt[0] = 1;
        nctt[1] = ncL;
        nctt[2] = ncL * ncL;
}

void Sim:: CalcularCel() {
        cout << "Calculando celdas " << endl;

        for (int icel =0; icel < nct; icel++) {
            // Se limpian las celdas al inicio
            celdas[icel] -> nac = 0;
            celdas[icel] -> iats.clear();
        }
        //cout << "Ya se limpio la celda" << endl;


        // Ciclo para todos los atomos
        for (int ia =0; ia < na; ia++) {
            int acel = 0;
            for (int id =0; id < nd; id++) {
                // ap es la posicion del atomo ap, en el valor de la x
                double ap = atomos[ia] -> p[id];
                // Se toma la funcion piso - entero del valor de la posicion, esta es la coordenada x.
                int api = ap;
                atomos[ia] -> ccel[id] = api; // Se guarda la coordenada del atomo
                acel = acel + api * nctt[id]; // Se calcula el numero de la celda
            }
            atomos[ia] -> ncel = acel; // Se guarda el numero de la celda
            celdas[acel] -> nac++; // Se incrementa el numero de atomos en la celda
            celdas[acel] -> iats.push_back(ia); // Se guarda el numero del atomo en la celda

            //cout << "Parte final " << endl;
        }
    }

void Sim::AceleracionesCel() {
        cout << "Calculando Aceleraciones cel  " << endl;
        //    cout << endl;
        //    cout << "Sim::Aceleraciones " << endl;

        for (int ia = 0; ia < na; ia++) {
            for (int id = 0; id < nd; id++) {
                atomos[ia] -> a[id] = 0.0;
            }
        }
        cout << "Se limpio exitosamente " << endl;
        double u=0;
        // El erro esta aca:
        // Agregar las condiciones periodicas y sobre las celdas que son negativas.
        for (int ia = 0; ia < na; ia++) {
            for (int iz = -nrc; iz <= nrc; iz ++) {
                double z = iz * nctt[2]; // La z es 0 la primera vez, pero luego va a ser el numero de celdas
                for (int iy = -nrc; iy <= nrc; iy ++) {
                    double y = iy * nctt[1];
                    for (int ix = -nrc; ix <= nrc; ix ++) {
                        double x = ix * nctt[0];
                        int ncel = x + y +z;
                        int nac = celdas[ncel] -> nac;
                        if (nac > 0 ) {
                            // AHora tomamos todos los atomos que estan en esta celda
                            // nac= numero de atomos en esa celda
                            for(int ja = 0; ja < nac; ja++) {
                                //
                                double r = Dist(ia,ja); // Se calculo la distancia
                                if(r < rc) {
                                    u = u + LJ(r);
                                    for(int id = 0; id < nd; id++) {
                                        atomos[ia] -> a[id] =
                                            atomos[ia] -> a[id] + f * dis[id] *0.5 ;
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }
        cout << "fase final acl " << endl;
}



