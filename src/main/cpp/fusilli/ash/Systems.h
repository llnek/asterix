// This library is distributed in  the hope that it will be useful but without
// any  warranty; without  even  the  implied  warranty of  merchantability or
// fitness for a particular purpose.
// The use and distribution terms for this software are covered by the Eclipse
// Public License 1.0  (http://opensource.org/licenses/eclipse-1.0.php)  which
// can be found in the file epl-v10.html at the root of this distribution.
// By using this software in any  fashion, you are agreeing to be bound by the
// terms of this license. You  must not remove this notice, or any other, from
// this software.
// Copyright (c) 2013-2015, Ken Leung. All rights reserved.

#if !defined(__SYSTEMS_H__)
#define __SYSTEMS_H__

#include "Ash.h"
NS_BEGIN(ash)

class FS_DLL SystemList {
public:

  System *head= nullptr;
  System *tail= nullptr;

  virtual ~SystemList() {

  }
  SystemList() {}

  NOCPYASS(SystemList)

        add: function( system ) {
            if( !this.head ) {
                this.head = this.tail = system;
                system.next = system.previous = null;
            } else {
                for( var node = this.tail; node; node = node.previous ) {
                    if( node.priority <= system.priority ) {
                        break;
                    }
                }
                if( node === this.tail ) {
                    this.tail.next = system;
                    system.previous = this.tail;
                    system.next = null;
                    this.tail = system;
                } else if( !node ) {
                    system.next = this.head;
                    system.previous = null;
                    this.head.previous = system;
                    this.head = system;
                } else {
                    system.next = node.next;
                    system.previous = node;
                    node.next.previous = system;
                    node.next = system;
                }
            }
        },

        remove: function( system ) {
            if ( this.head === system ) {
                this.head = this.head.next;
            }
            if ( this.tail === system ) {
                this.tail = this.tail.previous;
            }
            if ( system.previous ) {
                system.previous.next = system.next;
            }
            if ( system.next ) {
                system.next.previous = system.previous;
            }
        },

        removeAll: function() {
            while( this.head )
            {
                var system = this.head;
                this.head = this.head.next;
                system.previous = null;
                system.next = null;
            }
            this.tail = null;
        },

        get: function( type ) {
            for( var system = this.head; system; system = system.next ) {
                if ( system.is( type ) ) {
                    return system;
                }
            }
            return null;
        }
    });

    return SystemList;
});



NS_END(ash)
#endif



