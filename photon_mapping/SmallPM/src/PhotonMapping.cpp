/*********************************************************************************
Copyright (C) 2014 Adrian Jarabo (ajarabo@unizar.es)
Copyright (C) 2014 Diego Gutierrez (diegog@unizar.es)
All rights reserved.

This is an educational Ray Tracer developed for the course 'Informatica Grafica'
(Computer Graphics) tought at Universidad de Zaragoza (Spain). As such, it does not 
intend to be fast or general, but just to provide an educational tool for undergraduate
students. 

This software is provided as is, and any express or implied warranties are disclaimed.
In no event shall copyright holders be liable for any damage.
**********************************************************************************/
#define _USE_MATH_DEFINES
#include "PhotonMapping.h"
#include "World.h"
#include "Intersection.h"
#include "Ray.h"
#include "BSDF.h"
#include <random>
#include <cmath>

//*********************************************************************
// Compute the photons by tracing the Ray 'r' from the light source
// through the scene, and by storing the intersections with matter
// in the lists 'xx_photons', storing the diffuse (global) and caustic
// photons respectively. For efficiency, both are computed at the same
// time, since computing them separately would result into a lost of
// several samples marked as caustic or diffuse.
// Same goes with the boolean 'direct', that specifies if direct
// photons (from light to surface) are being stored or not.
// The initial traced photon has energy defined by the tristimulus
// 'p', that accounts for the emitted power of the light source.
// The function will return true when there are more photons (caustic
// or diffuse) to be shot, and false otherwise.
//---------------------------------------------------------------------

// TODO: explain thoroughly what the function does and why
/*
* @param r - rayo desde la luz a una direcci�n aleatoria
* @param p - energ�a, intensidad de la luz de la que parte
* @param global_photons - fotones de superficies difusas
* @param caustic_photons - fotones que se reflejaron en almenos una
		reflexi�n especular antes de hacerlo en una difusa
* @param direct	- iluminaci�n directa
* @param direct_only
* @return bool, devuelve tambi�n una lista de global_photons y caustic_photons
*/
bool PhotonMapping::trace_ray(const Ray& r, const Vector3 &p,
                              std::list<Photon> &global_photons, std::list<Photon> &caustic_photons, bool direct, bool direct_only)
{
#ifndef MAX_PHOTON_ITERATIONS
#define MAX_PHOTON_ITERATIONS 20
#endif

    // Check if max number of shots done...
    if( ++m_nb_current_shots > m_max_nb_shots )
    {
        return false;
    }

    // Compute irradiance photon's energy
    Vector3 energy(p);

    Ray photon_ray(r);
    photon_ray.shift(); // ???

    bool is_caustic_particle = false;

    // Itera un camino en una direcci�n
    while(1)
    {
        // Throw ray and update current_it
        Intersection it;
        world->first_intersection(photon_ray, it);

        if( !it.did_hit() )
            break;

        // Check if has hit a delta material...
        if( it.intersected()->material()->is_delta() )
        {
            // If delta material, then is caustic...
            // Don't store the photon!
            is_caustic_particle = true;
        }
        else if (photon_ray.get_level() > 0 || direct || direct_only )
        {
            // If non-delta material, store the photon!
            if( is_caustic_particle )
            {
                //If caustic particle, store in caustics
                if( caustic_photons.size() < m_nb_caustic_photons )
                    caustic_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
            }
            else
            {
                // If non-caustic particle, store in global
                if( global_photons.size() < m_nb_global_photons )
                    global_photons.push_back( Photon(it.get_position(), photon_ray.get_direction(), energy ));
            }
            is_caustic_particle = false;
        }

        Real pdf;

        Vector3 surf_albedo = it.intersected()->material()->get_albedo(it);
        Real avg_surf_albedo = surf_albedo.avg();

        Real epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);
        while (epsilon2 < 0.)
            epsilon2 = static_cast<Real>(rand())/static_cast<Real>(RAND_MAX);

        if (epsilon2 > avg_surf_albedo || photon_ray.get_level() > MAX_PHOTON_ITERATIONS )
            break;

        if (direct_only && !is_caustic_particle)
            break;

        // Random walk's next step
        // Get sampled direction plus pdf, and update attenuation
        it.intersected()->material()->get_outgoing_sample_ray(it, photon_ray, pdf );

        // Shade...
        energy = energy*surf_albedo;
        if( !it.intersected()->material()->is_delta() )
            energy *= dot_abs(it.get_normal(), photon_ray.get_direction())/3.14159;

        energy = energy /(pdf*avg_surf_albedo);
    }

    if( caustic_photons.size() == m_nb_caustic_photons &&
        global_photons.size() == m_nb_global_photons )
    {
        m_max_nb_shots = m_nb_current_shots-1;
        return false;
    }

    return true;
}

std::vector<Real> vector3_to_vector(const Vector3& vect)
{
    return { vect.data[0], vect.data[1], vect.data[2] };
}

//*********************************************************************
// TODO: Implement the preprocess step of photon mapping,
// where the photons are traced through the scene. To do it,
// you need to follow these steps for each shoot:
//  1 - Sample a world's light source in the scene to create
//		the initial direct photon from the light source.
//	2 - Trace the photon through the scene storing the inter-
//		sections between the photons and matter. You can use
//		the function 'trace_ray' for this purpose.
//	3 - Finally, once all the photons have been shot, you'll
//		need to build the photon maps, that will be used later
//		for rendering.
//		NOTE: Careful with function
//---------------------------------------------------------------------
void PhotonMapping::preprocess()
{
    // Elegir una de las fuentes de luz (puntual) (aleaotoriamente) y muestrear uniformemente alrededor de todas
    // las diercciones, una direcci�n de salida aleatoria
    // direcciones con un �ngulo s�lido de 4PI (el �ngulo que recoge todas las direcciones de una esfera)
    // posibilidad de hacerlo con:
    // - Rejection sampling o **sampling phi and theta (m�s eficiente ya que no se rechazan muestras)
    // IMP: tener en cuenta la probabilidad de cada direcci�n que sale ****(donde?)
    // La probabilidad de muestrear un punto �nico en una esfera, una direcci�n tiene que integrar a 1 en todas
    // las direcciones de la esfera posibles
    // es decir, la probabilidad de todas las direcciones que se muestreen tiene que ser 1 / 4PI
    // para trazar un photon que va a ir por la escena, mediante trace_ray
    // Guardar los photones en una estructura de aceleraci�n (kdTree)
    // https://stackoverflow.com/questions/5408276/sampling-uniformly-distributed-random-points-inside-a-spherical-volume
    // http://corysimon.github.io/articles/uniformdistn-on-sphere/
    // https://www.cs.princeton.edu/courses/archive/fall16/cos526/lectures/03-photonmapping.pdf *****

    std::random_device rd;
    std::default_random_engine random(rd());
    std::uniform_real_distribution<Real> distr_phi(0.0, 2.0 * M_PI);
    std::uniform_real_distribution<Real> distr_theta(-1.0, 1.0);
    std::uniform_int_distribution<int> distr_luz(0, world->nb_lights()-1);

    std::list<Photon> global_photons, caustic_photons;
    bool caminos_restantes = true;
    int numero_max_fotones = m_max_nb_shots / world->nb_lights();

    while(caminos_restantes || numero_max_fotones > 0) {

        // Sampling phi and theta
        Real phi = distr_phi(random);
        Real theta = acos(distr_theta(random));

        Real x = sin(theta) * cos(phi);
        Real y = sin(theta) * sin(phi);
        Real z = cos(theta);

        // Seleccionar una luz aleatoria
        LightSource* luz = world->light_source_list[distr_luz(random)];

        auto intensidad_de_cada_foton = luz->get_intensities() / (4*M_PI);

        // Crear rayo desde la luz a direcci�n aleatoria
        const Ray rayo_luz(luz->get_position(), Vector3(x, y, z));

        caminos_restantes = trace_ray(rayo_luz, intensidad_de_cada_foton, global_photons, caustic_photons, true, true);
        numero_max_fotones--;
    }

    cout << "caustic_photons " << caustic_photons.size() << endl;
    cout << "global_photons " << global_photons.size() << endl;

    // Almacenar los photones c�usticos y globales en el KdTree
    //KDTree<Photon, 3> m_global_map, m_caustics_map;
    //store(const std::vector<Real>& point, const T& data)
    for (Photon photon : caustic_photons) {
        // La intesidad de los photones (flux) depende del n�mero de photones emitidos, no en el
        // n�mero de photones en el photon map --> flux = flux/power
        // Flux, position, direction
        photon.flux = photon.flux / (global_photons.size() + caustic_photons.size());
        m_caustics_map.store(vector3_to_vector(photon.position), photon);
    }
    if(!caustic_photons.empty()) m_caustics_map.balance();

    for (Photon photon : global_photons) {
        photon.flux = photon.flux / (global_photons.size() + caustic_photons.size());
        m_global_map.store(vector3_to_vector(photon.position), photon);
    }
    if(!global_photons.empty()) m_global_map.balance();
}

//*********************************************************************
// TODO: Implement the function that computes the rendering equation
// using radiance estimation with photon mapping, using the photon
// maps computed as a proprocess. Note that you will need to handle
// both direct and global illumination, together with the
// recursive evaluation of delta materials. For an optimal implemen-
// tation you should be able to do it iteratively.
// In principle, the class is prepared to perform radiance estimation
// using k-nearest neighbors ('m_nb_photons') to define the bandwidth
// of the kernel.
//---------------------------------------------------------------------
Vector3 PhotonMapping::shade(Intersection &it0) const
{
    /*
        Buscar los k photones m�s cercanos al punto y calcular la estimaci�n de radiancia en base a las
        propiedades que nos proporcione la intersecci�n:
        - Funci�n de reflectancia de la superficie con un �ngulo de entrada
        - Y en base al �ngulo del photon, su energ�a y su probabilidad al salir de la fuente de luz
        - Y el tama�o del kernel que depender� radio m�ximo que den los k photones
        m�s cercanos de la estructura aceleraci�n (existe una funci�n que los devuelve)
        Computar la estimaci�n de radiancia en ese punto

        https://es.wikipedia.org/wiki/Mapeado_de_fotones

    */

    Vector3 L(0);
    Vector3 W(1);

    Intersection it(it0);

    int nb_bounces = 0;
    while(it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
    {
        Ray r; float pdf;
        it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf );
        W = W * it.intersected()->material()->get_albedo(it)/pdf;

        r.shift();
        world->first_intersection(r, it);
    }

    if(!it.did_hit()){
        //no colisiona o acaba en un material delta, devolvemos 0
        return Vector3(0);
    }

    Vector3 aporte_luz_directa = calculo_luz_directa(it);
    Vector3 aporte_fotones_causticos = calculo_nearest_neighbour(it, m_caustics_map);
    Vector3 aporte_fotones_globales = calculo_nearest_neighbour(it, m_global_map);


    //**********************************************************************
    // The following piece of code is included here for two reasons: first
    // it works as a 'hello world' code to check that everthing compiles
    // just fine, and second, to illustrate some of the functions that you
    // will need when doing the work. Goes without saying: remove the
    // pieces of code that you won't be using.
    //
    unsigned int debug_mode = 7;

    switch (debug_mode)
    {
        case 1:
            // ----------------------------------------------------------------
            // Display Albedo Only
            L = it.intersected()->material()->get_albedo(it);
            break;
        case 2:
            // ----------------------------------------------------------------
            // Display Normal Buffer
            L = it.get_normal();
            break;
        case 3:
            // ----------------------------------------------------------------
            // Display whether the material is specular (or refractive)
            L = Vector3(it.intersected()->material()->is_delta());
            break;

        case 4:
            // ----------------------------------------------------------------
            // Display incoming illumination from light(0)
            L = world->light(0).get_incoming_light(it.get_position());
            break;

        case 5:
            // ----------------------------------------------------------------
            // Display incoming direction from light(0)
            L = world->light(0).get_incoming_direction(it.get_position());
            break;

        case 6:
            // ----------------------------------------------------------------
            // Check Visibility from light(0)
            if (world->light(0).is_visible(it.get_position()))
                L = Vector3(1.);
            break;
        case 7:
            return aporte_luz_directa + aporte_fotones_causticos + aporte_fotones_globales;
            break;
        case 8:
            // ----------------------------------------------------------------
            // Reflect and refract until a diffuse surface is found, then show its albedo...
            int nb_bounces = 0;
            // MAX_NB_BOUNCES defined in ./smallrt/include/globals.h
            while( it.intersected()->material()->is_delta() && ++nb_bounces < MAX_NB_BOUNCES)
            {
                Ray r; float pdf;
                it.intersected()->material()->get_outgoing_sample_ray(it, r, pdf );
                W = W * it.intersected()->material()->get_albedo(it)/pdf;

                r.shift();
                world->first_intersection(r, it);
            }
            L = it.intersected()->material()->get_albedo(it);

    }
    return L*W;
}

Vector3 PhotonMapping::calculo_luz_directa(Intersection intersection) const {
    Vector3 radiancia(0);
    for(auto& luz : world->light_source_list){
        if(!luz->is_visible(intersection.get_position())){
            return(Vector3(0));
        }
        radiancia += PhotonMapping::radiancia(luz->get_incoming_light(intersection.get_position()),
                  -luz->get_incoming_direction(intersection.get_position()).normalize(),
                  intersection, -luz->get_incoming_direction(intersection.get_position()).length());
    }
    return radiancia;
}
/*
Vector3 PhotonMapping::radiancia(const KDTree<Photon, 3>::Node* foton, Intersection intersection, Real r) const {
    return intersection.get_normal().dot_abs(-foton->data().direction)  //n*wi
    * intersection.intersected()->material()->get_specular(intersection)   //Kmaterial
    * intersection.intersected()->material()->get_albedo(intersection)  //albedo
    * foton->data().flux / (M_PI*(r*r));
}
*/

Vector3 PhotonMapping::calculo_nearest_neighbour(Intersection intersection, KDTree<Photon, 3> tree) const {
    vector<const KDTree<Photon, 3>::Node*> fotones = vector<const KDTree<Photon, 3>::Node*>();
    Real distancia_max_alcanzada;
    Vector3 radiancia(0);
    tree.nearest_neighbor_search(vector3_to_vector(intersection.get_position()), MAX_NB_PHOTONS,
                                 fotones, distancia_max_alcanzada);

    for (const KDTree<Photon, 3>::Node*foton:fotones) {
        radiancia += PhotonMapping::radiancia(foton->data().flux,
                                              -foton->data().direction,
                                              intersection,
                                              distancia_max_alcanzada);
    }
    return(radiancia);
}

Vector3 PhotonMapping::radiancia(Vector3 luz, Vector3 wi, Intersection intersection, Real r) const {
    Vector3 a = intersection.get_normal().dot_abs(wi)  //n*wi
                * (intersection.intersected()->material()->get_specular(intersection) + 2)   //Kmaterial
                * intersection.intersected()->material()->get_albedo(intersection)  //albedo
                * luz / (M_PI*(r*r));
    return a;
}