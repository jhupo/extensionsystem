#ifndef __EXTENSION_FORMATTER_H__
#define __EXTENSION_FORMATTER_H__

#include <basicvariant/basicvariant.h>

#include <list>
#include <string>

namespace extension{

    namespace core{

        class FormatterPrivate;

        class EXTENSION_EXPORT Formatter : public VariantFormatter
        {
            DISABLE_COPY(Formatter)
            DECLARE_PRIVATE(Formatter)
            DECLARE_SHARED_PTR(Formatter)
        public:

            Formatter(const BasicVariant& var = basic_variant_null);
            virtual~Formatter();

            void addChildren(const ptr& children);
            void addChildren(const Formatter& children);
            void addVariant(const BasicVariant& var);
            void addVariant(const BasicVariant& first, const BasicVariant& second);
            void addVariant(const BasicVariant& first, const std::list<std::pair<BasicVariant,BasicVariant> >& second);

            void reset(const std::list<ptr>& children);
            void reset(const std::list<BasicVariant>& args);
            void reset(const std::list<std::pair<BasicVariant,BasicVariant> >& kargs);

            std::list<ptr> children()const;
            std::list<BasicVariant> args()const;
            std::list<std::pair<BasicVariant,BasicVariant> > kargs()const;

            ptr findChildren(const BasicVariant& var)const;
            BasicVariant findArgs(const BasicVariant& var)const;
            BasicVariant findKargs(const BasicVariant& var)const;

            std::list<BasicVariant> kargsMember()const;

            bool isArgsMember(const BasicVariant& var)const;
            bool isKargsMember(const BasicVariant& var)const;
            bool isChildrenMember(const BasicVariant& children)const;

            void eraseArgs(const BasicVariant& var);
            void eraseKargs(const BasicVariant& var);
            void eraseChildren(const BasicVariant& children);

            void clear();

        private:
            const std::shared_ptr<FormatterPrivate> d_ptr;
        };

        typedef std::list<std::pair<BasicVariant,BasicVariant> > FormatterKargs;
        typedef std::list<BasicVariant> FormatterArgs;
    }

}



#endif