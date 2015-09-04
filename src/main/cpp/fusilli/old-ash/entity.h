#if !defined(__ENTITY_H__)
#define __ENTITY_H__

class CC_DLL Entity {
        previous:null, /* Entity */
        next: null, /* Entity */
        components: null,

        constructor: function ()  {
            this.components = new Dictionary();
            this.componentAdded = new signals.Signal();
            this.componentRemoved = new signals.Signal();
        },

        add: function (component, componentClass ) {
			if( typeof componentClass === "undefined" )
			{
				componentClass = component.constructor;
			}
            if ( this.components.has( componentClass ) )
			{
                this.remove( componentClass );
            }
            this.components.add(componentClass, component);
            this.componentAdded.dispatch( this, componentClass );
            return this;
        },

        remove: function ( componentClass ) {
            var component = this.components.retrieve( componentClass );
            if ( component ) {
                this.components.remove( componentClass );
                this.componentRemoved.dispatch( this, componentClass );
                return component;
            }
            return null;
        },

        get: function (componentClass) {
            return this.components.retrieve( componentClass );
        },

        /**
         * Get all components from the entity.
         * @return {Array} Contains all the components on the entity
         */
        getAll: function () {
            var componentArray = [];
            this.components.forEach(function( componentObject, component ) {
                componentArray.push(component);
            });
            return componentArray;
        },

        has: function (componentClass) {
            return this.components.has( componentClass );
        }
};


#endif

