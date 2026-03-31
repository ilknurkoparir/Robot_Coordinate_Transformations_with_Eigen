# Uzaysal Dönüşümler



---


## 📌 Problem Tanımı

Laboratuvar ortamında elde edilen sensör verileri, sensörün kendi koordinat sistemine göre tanımlıdır ve gerçek dünya koordinat sistemindeki konumları doğrudan bilinmemektedir. Bu problemi çözmek için çeşitli dönüşüm fonksiyonları kullanılarak veriler ortak bir referans sistemine dönüştürülür. Dönüşüm sonrasında tüm nokta bulutları aynı dünya koordinat sistemine ifade edildiğinden, farklı zamanlarda elde edilen veriler birleştirilerek ortamın tutarlı bir 3B haritası oluşturulabilir.

---


## ⚙️ Kullanılan Fonksiyonlar

Tüm implementasyonlar **Eigen** kütüphanesi kullanılarak `transforms.cpp` dosyasında gerçekleştirilmiştir.

| Fonksiyon | Girdi | Çıktı | Açıklama |
|:----------|:------|:------|:---------|
| `convertZYX2Q` | ZYX (3×1) | q (4×1) | ZYX Euler açıları → Quaternion |
| `convertQ2ZYX` | q (4×1) | ZYX (3×1) | Quaternion → ZYX Euler açıları |
| `getRotation(Vector3d)` | ZYX (3×1) | R (3×3) | ZYX → Rotasyon matrisi |
| `getRotation(Matrix3d)` | R (3×3) | ZYX (3×1) | Rotasyon matrisi → ZYX |
| `getRotation(Vector4d)` | q (4×1) | R (3×3) | Quaternion → Rotasyon matrisi |
| `getRotation(Matrix3d, bool)` | R (3×3) | q (4×1) | Rotasyon matrisi → Quaternion |
| `transform(T1, T2)` | T₁, T₂ (4×4) | T (4×4) | T = T₁ · T₂ |
| `transform(T, p)` | T (4×4), p (3×1) | p′ (3×1) | p′ = T · [p; 1] |
| `makeHomogeneous(R, t)` | R (3×3), t (3×1) | T (4×4) | Homojen matris oluştur |

> **Referans:** Bruno Siciliano et al., *Robotics: Modelling, Planning and Control*, Springer 2009.

---

## 🔨 Derleme ve Çalıştırma

### Gereksinimler

- C++14 veya üzeri
- [Eigen 3](http://eigen.tuxfamily.org/) kütüphanesi

```bash
# Ubuntu / Debian
sudo apt-get install libeigen3-dev
```



### Girdi / Çıktı Dosyaları

| Girdi | Çıktı |
|:------|:------|
| `4527-pointcloud.xyz` | `4527-pointcloud_w.xyz` |
| `4723-pointcloud.xyz` | `4723-pointcloud_w.xyz` |
| `4768-pointcloud.xyz` | `4768-pointcloud_w.xyz` |
| `4798-pointcloud.xyz` | `4798-pointcloud_w.xyz` |

---


## 📊 Sonuçlar

### Dönüşüm Öncesi — Sensör Koordinat Sisteminde (S Frame)

Nokta bulutları, SICK sensörüne göre tanımlı yerel koordinat sisteminde görüntülenmektedir. Her zaman adımındaki nokta bulutu **birbirinden bağımsız** ve sensöre göre konumlandırılmış olarak görünmekte; UAV'ın gerçek uzaydaki konumu yansıtılmamaktadır.

<img width="1920" height="1080" alt="Dönüşüm Öncesi - Sensör Koordinat Sistemi" src="https://github.com/user-attachments/assets/8283fbfe-b803-4460-af35-a0820e92c3d0" />

---

### Dönüşüm Sonrası — Dünya Koordinat Sisteminde (W Frame)

Nokta bulutları, OptiTrack'in dünya koordinat sistemine (T_S^W dönüşüm matrisi ile) aktarılmıştır. Artık tüm zaman adımlarındaki bulutlar **aynı referans sisteminde** hizalanmış olup UAV'ın gerçek uçuş yolu ve ortamın 3B yapısı gözlemlenebilmektedir.

<img width="1920" height="1080" alt="Dönüşüm Sonrası - Dünya Koordinat Sistemi" src="https://github.com/user-attachments/assets/ef2eadee-ed6c-4d23-bdf6-12c53ee4568c" />

---

### Farkın Açıklaması

| | Dönüşüm Öncesi | Dönüşüm Sonrası |
|:--|:--------------|:----------------|
| **Referans** | Sensör (S Frame) | Dünya (W Frame) |
| **Konum** | Sensöre göre yerel | OptiTrack'e göre global |
| **Hizalama** | Bulutlar dağınık / üst üste | Bulutlar uzamsal olarak doğru konumda |
| **UAV hareketi** | Görünmez | X ekseni boyunca ilerleme seçilebilir |
| **Kullanım** | Tek kare analizi | Haritalama, odometri, 3B yeniden yapılandırma |

> **Özet:** Dönüşüm öncesinde her nokta bulutu kendi sensör eksenine göre tanımlıdır ve gerçek uzayda nerede olduğu bilinmez. Dönüşüm sonrasında ise tüm bulutlar aynı dünya koordinat sistemine taşındığından, farklı zaman dilimlerinde toplanan veriler ortamın tutarlı bir 3B haritasını oluşturmak için kullanılabilir.

---

## 📚 Kaynaklar

- Bruno Siciliano, Lorenzo Sciavicco, Luigi Villani, Giuseppe Oriolo — *Robotics: Modelling, Planning and Control*, Springer 2009
- [Eigen Kütüphanesi](http://eigen.tuxfamily.org/)
- [CloudCompare](https://www.cloudcompare.org/)
