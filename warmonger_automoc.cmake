#
# Copyright (C) 2015-2016 Botond Dénes
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

# Create a rule to generate the moc file for each source file that uses
# either the Q_OBJECT or Q_GADGET macros.
# The function assumes (for now) that all thes macros are used exclusively in
# the header files and that the header files have the same name as the cpp
# files, using the .h extension instead of the .cpp.
# You may nead to run cmake manually if you added the Q_OBJECT or Q_GADGET
# macros to existing files that didn't have it before.
# Generated moc files are placed in a directory hierarchy that mimics the
# hierarchy of the source tree to avoid name clashes.
function(WARMONGER_AUTOMOC target)
    message(STATUS "Generating moc targets for ${target}")
    get_target_property(target_sources ${target} SOURCES)

    set(target_new_sources ${target_sources})

    set(moc_prefix "${CMAKE_BINARY_DIR}/moc/${target}")
    file(MAKE_DIRECTORY ${moc_prefix})

    foreach(src ${target_sources})
        get_filename_component(src_dir ${src} DIRECTORY)
        get_filename_component(src_name ${src} NAME_WE)
        get_filename_component(src_ext ${src} EXT)

        # Examine the cpp file
        file(READ ${src} src_content)

        set(qobject_pos -1)
        set(qgadget_pos -1)
        string(FIND "${src_content}" "Q_OBJECT" qobject_pos)
        string(FIND "${src_content}" "Q_GADGET" qgadget_pos)

        if(${qobject_pos} GREATER -1 OR ${qgadget_pos} GREATER -1)
            set(src_moc ${moc_prefix}/${src_name}.moc)
            QT5_GENERATE_MOC(${src} "${src_moc}")
            list(APPEND target_new_sources "${src_moc}")
        endif()

        # Examine the h file
        set(src_header ${src_dir}/${src_name}.h)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src_header}")
            file(READ ${src_header} header_content)

            set(qobject_pos -1)
            set(qgadget_pos -1)
            string(FIND "${header_content}" "Q_OBJECT" qobject_pos)
            string(FIND "${header_content}" "Q_GADGET" qgadget_pos)

            if(${qobject_pos} GREATER -1 OR ${qgadget_pos} GREATER -1)
                set(src_moc ${moc_prefix}/moc_${src_name}${src_ext})
                QT5_GENERATE_MOC(${src_header} "${src_moc}")
                list(APPEND target_new_sources "${src_moc}")
            endif()
        endif()
    endforeach()

    set_target_properties(${target} PROPERTIES SOURCES "${target_new_sources}")
endfunction()
