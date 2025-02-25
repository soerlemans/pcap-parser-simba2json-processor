#ifndef VIEW_CAST_HPP
#define VIEW_CAST_HPP

/*!
 * Cast a view (const pointer) to another view type.
 * This is mean to shorten code that extensively uses @ref reinterpret_cast.
 */
template <typename CastType, typename ViewType>
inline constexpr auto view_cast(const ViewType* t_ptr) -> const CastType* {
  return reinterpret_cast<const CastType*>(t_ptr);
}

#endif // VIEW_CAST_HPP
