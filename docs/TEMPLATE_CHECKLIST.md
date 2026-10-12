# Checklist Kelayakan Template

## Status saat ini

Template sudah memiliki fondasi yang dapat di-clone:

- Target board dan framework jelas.
- Versi platform PlatformIO dipin.
- Clean Architecture ringan.
- Feature flags untuk menjaga ukuran firmware.
- Contoh buildable untuk UART1, TIM4, LED, dan cooperative scheduler.
- Dokumentasi struktur, API, periferal, dan penambahan fitur.

Namun statusnya masih **template eksperimental**, belum template produksi universal.

## Wajib sebelum publikasi

- [ ] Tentukan lisensi dan tambahkan file `LICENSE`.
- [ ] Ganti nama/deskripsi repository agar tidak memakai nama eksperimen.
- [ ] Tandai repository sebagai Template repository di GitHub.
- [ ] Pastikan README memakai URL repository sebenarnya.
- [ ] Lakukan upload dan smoke test pada hardware fisik.
- [ ] Verifikasi output UART setelah restrukturisasi terakhir.
- [ ] Dokumentasikan model/clone ST-LINK dan USB-to-TTL yang diuji.
- [ ] Tambahkan release/tag awal, misalnya `v0.1.0`.

## Sangat disarankan

- [ ] Tambahkan GitHub Actions untuk menjalankan `pio run` pada push dan pull request.
- [ ] Tambahkan unit test untuk domain/fungsi murni.
- [ ] Tambahkan contoh aplikasi terpisah, bukan menambah banyak fungsi ke `app.c` default.
- [ ] Tambahkan daftar pin board dan konflik alternate function.
- [ ] Tambahkan format kode yang konsisten dan pemeriksaan otomatis.
- [ ] Tambahkan `CONTRIBUTING.md` jika menerima kontribusi.
- [ ] Tambahkan `CHANGELOG.md` setelah memiliki pengguna/release.

## Kekurangan teknis saat ini

- UART1 baru memiliki API TX blocking; RX, interrupt, ring buffer, dan timeout belum tersedia.
- Scheduler masih cooperative tanpa priority atau preemption; callback yang lama menambah latency task lain.
- UART1 TX masih blocking walaupun clock TIM4 tetap berjalan melalui interrupt.
- GPIO, ADC1, I2C, SPI, dan timer lain baru memiliki startup scaffolding, belum API operasional lengkap.
- Error handling belum distandardisasi.
- Tidak ada perlindungan pointer `NULL` untuk menghemat kode.
- Tidak ada pengujian otomatis atau hardware-in-the-loop.
- Konfigurasi beberapa periferal masih berupa default hard-coded dalam driver masing-masing.
- Application contoh masih menerima tipe platform secara langsung; application ports belum diterapkan.

## Kriteria template stabil

Template dapat disebut stabil ketika:

1. Build otomatis selalu dijalankan di CI.
2. Contoh default berhasil di-upload dan diuji pada board nyata.
3. API yang dinyatakan didukung memiliki dokumentasi dan pengujian.
4. Perubahan breaking mengikuti versi/release yang jelas.
5. Lisensi dan batas dukungan hardware dinyatakan eksplisit.

Tidak perlu menyelesaikan seluruh driver periferal sebelum publikasi. Yang penting, bedakan dengan jelas antara `defined`, `scaffolded`, `tested`, dan `production-ready`.
