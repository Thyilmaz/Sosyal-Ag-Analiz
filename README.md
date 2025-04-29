# Social Network Analysis in C

Bu proje, C programlama dili kullanılarak geliştirilen bir **sosyal ağ analiz sistemidir**. Program, kullanıcılar arasındaki ilişkileri analiz etmek ve sosyal bağlantıları keşfetmek için çeşitli algoritmalar ve veri yapıları içerir. Proje kapsamında **kırmızı-siyah ağaçlar**, **derinlik öncelikli arama (DFS)** ve diğer algoritmalar kullanılmıştır.

## Özellikler

- 🔍 **İlişki Ağacı Oluşturma:** Kullanıcılar arasında bağlantılar kurulabilir.
- 👥 **İkinci Derece Arkadaş Bulma:** Her kullanıcının dolaylı arkadaşları tespit edilir.
- 🤝 **Ortak Arkadaş Analizi:** Her kullanıcı çifti için ortak arkadaş sayısı hesaplanır.
- 📡 **Etki Alanı Hesaplama:** Her bir kullanıcının sosyal ağ üzerindeki etkisi ölçülür.
- 🌐 **Topluluk Analizi:** DFS algoritmasıyla ağ üzerindeki topluluklar (bağlı bileşenler) belirlenir.
- 🌲 **Kırmızı-Siyah Ağaç Kullanımı:** Verimli kullanıcı ekleme, silme ve arama işlemleri gerçekleştirilir.

## Kullanım

Programın derlenmesi için bir C derleyicisine ihtiyacınız vardır (örneğin `gcc`).

```bash
gcc -o social_network main.c
./social_network
```

> Not: `main.c` dosyasının adı ve proje yapısı sizin yerel ortamınıza göre değişebilir.

## Gereksinimler

- C99 veya üzeri bir C derleyicisi
- Standart C kütüphaneleri (ekstra bağımlılık yok)

## Lisans

Bu proje MIT lisansı ile lisanslanmıştır. Daha fazla bilgi için `LICENSE` dosyasına göz atabilirsiniz.
