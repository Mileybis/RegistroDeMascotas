include("/home/Saimon/Documentos/Proyectos-git/RegistroDeMascotas/build/Desktop-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/RegistroDeMascotas-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "/home/Saimon/Documentos/Proyectos-git/RegistroDeMascotas/build/Desktop-Debug/RegistroDeMascotas"
    GENERATE_QT_CONF
)
