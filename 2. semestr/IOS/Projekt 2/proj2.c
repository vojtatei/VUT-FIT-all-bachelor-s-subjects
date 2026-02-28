--- proj2.c.odevzdany	2023-05-09 15:06:21.101067400 +0200
+++ proj2.c	2023-05-09 16:36:50.037593900 +0200
@@ -273,9 +273,10 @@
     // Pokud je pošta zavřená, zákazník skončí,
     // vypíše se hláška a proces se ukončí. Uvolní se semafory
     // pro zákazníky a úředníky
+    sem_wait(output);
     if (pamet->otevreno == 0)
     {
-        sem_wait(output);
+        
         
         ++pamet->A_counter;
         --pamet->zakaznik;
@@ -287,39 +288,39 @@
     }
     
     // Vygeneruje se náhodné ID služby
-    int id_sluzba = ((rand()+getpid()) % 3) + 1;
+    int id_sluzba = ((rand()+getpid()) % 3);
    //int id_sluzba = 1;
 
     ++pamet->pocet_sluzeb[id_sluzba];
 
-    sem_post(sem_zakaznik);
-    sem_wait(sem_urednik);
 
-    sem_wait(output);
+ 
     ++pamet->A_counter;
 
     // Zákazník vstoupil na poštu, vypíše se hláška
     // a uvolní se semafor
-    fprintf(output_file, "%d: Z %d: entering office for a service %d\n", pamet->A_counter,zakaznik_id, id_sluzba);
+    fprintf(output_file, "%d: Z %d: entering office for a service %d\n", pamet->A_counter,zakaznik_id, id_sluzba+1);
     sem_post(output);
 
+    sem_post(sluzba[id_sluzba]);
+    sem_post(sem_zakaznik);
+    sem_wait(sem_urednik);
 
     // Zákazník byl zavolán k uředníkovi
     sem_wait(output);
     ++pamet->A_counter;
-    --(pamet->pocet_sluzeb[id_sluzba]);
+    //--(pamet->pocet_sluzeb[id_sluzba]);
 
     fprintf(output_file,"%d: Z %d: called by office worker\n", pamet->A_counter, zakaznik_id);
 
     sem_post(output);
 
-    sem_post(sluzba[id_sluzba]);
 
     sem_post(sem_Z_done);
     sem_wait(sem_U_done);
 
     // Zákzník spí v intervalu <0,10>
-    usleep((rand() % 11) * 1000);
+    //usleep((rand() % 11) * 1000);
 
     // Zákazník se chystá odejít, vypíše se hláška a uvolní semafor
     sem_wait(output);
@@ -366,7 +367,7 @@
 
         // Pokud ve frontě není zákazník a pošta je otevřená, tak
         // si vezme přestávku
-        else if (pamet->zakaznik == 0 && pamet->otevreno == 1)
+        else if (pamet->pocet_sluzeb[0] == 0 && pamet->pocet_sluzeb[1] == 0 && pamet->pocet_sluzeb[2] == 0 && pamet->otevreno == 1)
         {
             ++pamet->A_counter;
             fprintf(output_file, "%d: U %d: taking break\n", pamet->A_counter, urednik_id);
@@ -374,6 +375,7 @@
 
             // Pokud je zadaná nenulová doba přestávky, tak si
             // uředník vezme přestávku v náhodném romezí <0,TU>
+            srand(time(NULL) + getpid());
             if (args.TU_max_time != 0)
             {
                 usleep(((rand() + getpid()) % args.TU_max_time * 1000));
@@ -397,10 +399,11 @@
         }
     
     // Vytvoření náhodného ID pro typ služby, která bude obsluhována
-    int id_sluzba = ((rand()+getpid()) % 3) + 1;
+    
+    int id_sluzba = 0;while(pamet->pocet_sluzeb[id_sluzba] == 0) {id_sluzba = ((rand()+getpid()) % 3);}
 
     // Kontroluje zda je nějaká služba k dispozici
-    if (pamet->pocet_sluzeb[0] > 0 || pamet->pocet_sluzeb[1] > 0 || pamet->pocet_sluzeb[2] > 0)
+    if (pamet->pocet_sluzeb[id_sluzba] > 0)
 
     {
         //int id_sluzba = 1;
@@ -412,7 +415,7 @@
 
         // Zvýši počítadlo akcí a vypíše zprávu o obsluze služby
         ++pamet->A_counter;
-        fprintf(output_file, "%d: U %d: serving service of type %d\n", pamet->A_counter, urednik_id, id_sluzba);
+        fprintf(output_file, "%d: U %d: serving service of type %d\n", pamet->A_counter, urednik_id, id_sluzba+1);
 
         sem_post(output);
         sem_wait(sem_Z_done);
