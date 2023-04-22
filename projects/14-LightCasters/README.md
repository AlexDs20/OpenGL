# Light casters

## Directional Light
- light source is infinitely far = directional light
    - light rays parallel
    - light direction always the same &rarr; same for all objects in the scene
    - define direction vector instead of a position vector

- Good for global lights

- Generally lightDir is **vec3** but could be set **vec4** with w = 0 (so that translation has no effect)
    - if working with a position of light however &rarr; w = 1
    - w = 0 or w = 1 can be used to check if it is a position or directional light and compute the right lightDirection



## Point Light
- Point that illuminates in all directions
- Rays fade over distance (constant flux through area that increases $dS = r^2 \sin\theta * d\theta * d\phi$)

In practice:
- Use attenuation:
    $$F_{att} = \frac{1.0}{K_c + K_l * d + K_q * d^2}$$
with:
    - $d$: distance from fragment to light source
    - $K_c = 1$, $K_l$, $K_q$: constant, linear and quadratic constants
