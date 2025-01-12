# 🛒  eshop σε C

---

## 📜 Περιγραφή

Αυτό το πρόγραμμα υλοποιεί ένα απλό **eshop** που εξυπηρετεί παραγγελίες μέσω διασύνδεσης **πελατών** και ενός **διακομιστή**. Η επικοινωνία πραγματοποιείται με τη χρήση pipes, και το σύστημα προσομοιώνει την αλληλεπίδραση μεταξύ πελατών και καταστήματος.

---

## ⚙️ Λειτουργικότητα

### Κατάλογος Προϊόντων
- Κάθε προϊόν έχει:
  - Τιμή
  - Απόθεμα (item_count = 2)

### 👥 Πελάτες
- **5 child processes** μέσω της `fork()`.
- Κάθε πελάτης υποβάλλει **10 παραγγελίες**, μία κάθε δευτερόλεπτο (`sleep(1)`).
- Οι παραγγελίες επιλέγουν τυχαία προϊόν από τον κατάλογο μέσω της `rand() % 20`.

### 🔄 Επικοινωνία
- Χρησιμοποιούνται **2 pipes** ανά πελάτη:
  1. Pipe παραγγελίας: Ο πελάτης γράφει τις παραγγελίες.
  2. Pipe απάντησης: Ο διακομιστής επιστρέφει την απάντηση.

- Διαδικασία:
  - Οι πελάτες γράφουν τις παραγγελίες στο πρώτο pipe.
  - Ο διακομιστής διαβάζει την παραγγελία, την επεξεργάζεται, και απαντά μέσω του δεύτερου pipe.

### 🛠️ Χειρισμός Παραγγελιών
- Ο server:
  - Επεξεργάζεται **1 παραγγελία ανά δευτερόλεπτο** (`sleep(1)`).
  - Ελέγχει τη διαθεσιμότητα του προϊόντος.
  - Απαντά με **επιτυχία** ή **αποτυχία**.

### 📊 Summary
Μετά την ολοκλήρωση όλων των παραγγελιών, ο διακομιστής δημιουργεί μια αναφορά που περιλαμβάνει:

   - Σύνολο παραγγελιών
   - Επιτυχείς παραγγελίες
   - Αποτυχημένες παραγγελίες
   - Συνολικός τζίρος

---

## 🚀 Τρόπος Χρήσης
1. Κλωνοποιήστε το αποθετήριο:

   ```git clone https://github.com/dehmitros/uth-eshop.git```
2. ```cd uth-eshop```
3. ```make```
4. ```./main```

## 📜 LICENSE
[License](https://github.com/dehmitros/uth-eshop/blob/master/LICENSE.md)
