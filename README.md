# __Konspekt gry__  + readme na samym dole
Tutaj nowy rozdział, ale już poprawiony
##
# 1. Ogolne zasady - dla dwóch graczy - przebieg rund:

### Przebieg mojej tury:
| **A) Trafiam** | **B) Nie trafiam** |
| --- | --- |
| 1. Biegnę podnieść puszkę | 1. Punkt dla przeciwnika |
| 2. Przeciwnik pije swoje piwo | 2. Koniec mojej tury |
| 3. Wracam z puszką do swojej bazy – przeciwnik nadal pije |  |
| 4. Przeciwnik przestaje pić w momencie, gdy postawię puszkę pionowo w bazie |  |
| 5. Koniec mojej tury |  |
## 
### Przebieg tury przeciwnika:
| **C) Przeciwnik trafia** | **D) Przeciwnik nie trafia** |
| --- | --- |
| 1. Zaczynam pić piwo | 1. Punkt dla mnie |
| 2. Kończę pić w momencie, gdy przeciwnik wróci i postawi puszkę w swojej bazie | 2. Koniec tury przeciwnika |
| 3. Koniec tury przeciwnika |  |

##
##
# 2. Koncepcje zdarzeń poszczególnych stanów:

### Scanariusz A:
| Zdarzenie | Akcja |
| --- | --- |
| Rzut puszką | Strzałkami ustawiam wektory rzutu |
| Start i bieg do puszki | Nadupcam w spację |
| Podniesienie puszki | Kombinacja klawiszy: np. G + P |
| Powrót do bazy | Nadupcam w spację |
| Postawienie puszki w bazie | Kombinacja klawiszy (np. H + O) |
| Zakończenie tury | Automatyczneie |

##
### Scanariusz B:
| Zdarzenie | Akcja |
| --- | --- |
| Rzut puszką | Strzałkami ustawiam wektory rzutu |
| Nietrafienie puszki | Brak akcji |
| Przyznanie punktu przeciwnikowi | Dodanie pktu dla komputera |
| Zakończenie mojej tury | -- |

##
### Scanariusz C:
| Zdarzenie | Akcja |
| --- | --- |
| Rzut puszką przez komputer | Silnik losuje parametry rzutu |
| Trafienie puszki | Uruchomienie timera biegu |
| Start picia piwa | Pasek piwa się pojawia; gracz musi nadupcac klawisz np. P - inaczej picie pauzuje |
| Picie piwa | Co 0.5 s pasek piwa maleje o wartość miedzy 3–5%, zalezna od: ___ilosc uderzen w klawisz P / dt___ |
| Bieg przeciwnika (w tle) | animacja biegu |
| Stop picia piwa | Timer picia się zatrzymuje |
| Zakończenie tury przeciwnika | Przejście do tury gracza; ilosc wypitego piwa co runde sie sumuje |

##
### Scanariusz D:

| Zdarzenie | Akcja |
| --- | --- |
| Rzut puszką przez komputer | Silnik losuje parametry rzutu |
| Nietrafienie puszki | Brak akcji |
| Przyznanie punktu dla gracza | Dodanie punktu dla gracza |
| Zakończenie tury przeciwnika | -- |

##
##
# 3. Dodatkowe założenia:
### 1. W trakcie picia piwa &lt;_przez gracza_&gt; mozna zrobic wypicie więcej niż jednego piwa.
### 2. Mozna wybrać poziom trudności bota - bot moze np:  
* czesciej trafiac
* szybciej biec po ustawienie swojej puszki
* cokolwiek innego co przyjdzie do glowy
### 3. Zwiększenie własnych umiejętności poprzez:
* kompletnie nie mam pomysłu, ale rzeczy typu lepsze piwa czy coś były mówione - to ma jakiś sens, I guess
### 4. Dodatkowe założenia zrealizowałbym po ukończeniu szkieletu, czyli tego co jest w tabelce
### 5. Pomyślałem jeszcze np ze mozna trafic w bota i za to dostac pkty XDDD
##
###
###
##



# 4. README FLANKI PROJECT ©

![SFML](https://img.shields.io/badge/SFML-3.0-green) ![C++](https://img.shields.io/badge/C++-17-blue)

## 🥸 Konfiguracja Projektu (Visual Studio)

🧿👄🧿

Aby poprawnie skonfigurować projekt, otwórz **Właściwości Projektu** (*Project Properties*) i wprowadź poniższe ustawienia.

### 1. Pliki Nagłówkowe (Include)
Nawiguj do:
`C/C++` -> `Ogólne` -> `Dodatkowe katalogi plików nagłówkowych`

> **Wartość:**
> ```text
> Ścieżka do SFML-3.0.0\include
> ```

### 2. Biblioteki (Linker)
Nawiguj do:
`Konsolidator` -> `Ogólne` -> `Dodatkowe katalogi biblioteki`

> **Wartość:**
> ```text
> Ścieżka do SFML-3.0.0\lib
> ```

### 3. Zależności (Dependencies)
Nawiguj do:
`Konsolidator` -> `Dane Wejściowe` -> `Dodatkowe zależności`

Skopiuj i wklej poniższy ciąg:
```text
$(CoreLibraryDependencies);%(AdditionalDependencies);sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib
```

### Zaleśnośći w release
```text
$(CoreLibraryDependencies);%(AdditionalDependencies);sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib
```
