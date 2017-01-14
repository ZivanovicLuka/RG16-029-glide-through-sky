# Glide Through Sky

![Unable to show Screenshot](https://github.com/MATF-RG16/RG16-029-glide-through-sky/blob/master/screenshots/screenshot006.png?raw=true)

### :package: Installation
Navigate to the project root folder (where the **Makefile** is located), and compile the code by calling **make**:
```
make
```
**Note:** For **make** to work, you must have the OpenGL/GLUT/Freeglut libraries installed.
You can find them **[here](https://www.opengl.org/resources/libraries/glut/)**.

After that, run **Glide Through Sky** executable:

```
./glideThroughSky.out
```

***
###:video_game: Controls

<table>
  <tr>
    <th colspan="2">Global</th>
  </tr>
  <tr>
    <td>G</td><td>Start Game</td>
  </tr>
  <tr>
    <td>S</td><td>Pause Game</td>
  </tr>
</table>

<table>
  <tr>
    <th colspan="2">Player 1</th>
  </tr>
  <tr>
    <td>Q</td><td>Jump</td>
  </tr>  
  <tr>
    <td>W</td><td>Dash</td>
  </tr>
  <tr>
    <td>E</td><td>Heal</td>
  </tr>
</table>
***

### :computer: Gameplay
* __Mana is represented by circle around player__  
__Health is represented by trail behind player__
  
* You start with 70% hp  

* Heal gives you 30% hp, and taking 1 mana  
Dash cost is 1 mana

* Taking bullet removes 15% of your hp  
Taking bullet while dashing removes 7% of your hp  
Hitting enemy removes 50% of your hp  
Hitting wall removes 100% of your hp

* Hitting mana crystal gives you 1 mana  
Hitting mana crystal whiel dashing gives you 1 mana (not losing for dash)  
Hitting enemy with dash gives you 1-2 mana

![Unable to show Screenshot](https://github.com/MATF-RG16/RG16-029-glide-through-sky/blob/master/screenshots/screenshot007.gif?raw=true)  
[**Download gameplay video**](https://github.com/MATF-RG16/RG16-029-glide-through-sky/blob/master/screenshots/Gameplay.mp4?raw=true)
